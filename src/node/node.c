#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
#include "server.h"
#include "client.h"

#define MAX_PEERS   32
#define BUF_SIZE    1024
#define MAX_POLL    (MAX_PEERS + 2)   /* listen fd + stdin + peers */

static int peers[MAX_PEERS];
static int peer_count = 0;

static int add_peer(int fd)
{
    if (peer_count >= MAX_PEERS) {
        fprintf(stderr, "[Node] Peer table full\n");
        return -1;
    }
    peers[peer_count++] = fd;
    return 0;
}

static void remove_peer(int index)
{
    server_close(peers[index]);
    peers[index] = peers[--peer_count];
}

static void broadcast(const char *msg, size_t len)
{
    for (int i = 0; i < peer_count; i++) {
        if (send_data(peers[i], msg, len) < 0) {
            fprintf(stderr, "[Node] Failed to send to peer fd %d\n", peers[i]);
        }
    }
}

/* Parse "ip:port" in place. Returns 0 on success. */
static int parse_peer(char *arg, char **ip, int *port)
{
    char *colon = strrchr(arg, ':');
    if (colon == NULL || colon == arg || colon[1] == '\0') {
        return -1;
    }
    *colon = '\0';
    char *end = NULL;
    long p = strtol(colon + 1, &end, 10);
    if (*end != '\0' || p < 1 || p > 65535) {
        return -1;
    }
    *ip = arg;
    *port = (int)p;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <listen_port> [peer_ip:peer_port ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *end = NULL;
    long port_l = strtol(argv[1], &end, 10);
    if (*end != '\0' || port_l < 1 || port_l > 65535) {
        fprintf(stderr, "Invalid port: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    int port = (int)port_l;

    signal(SIGPIPE, SIG_IGN);

    /* 1. Server role: listen for peers */
    int server_fd = start_server_module(port);
    if (server_fd < 0) {
        fprintf(stderr, "[Node] Failed to start server module\n");
        return EXIT_FAILURE;
    }

    /* 2. Client role: connect to the peers given on the command line */
    for (int i = 2; i < argc; i++) {
        char *ip = NULL;
        int peer_port = 0;
        if (parse_peer(argv[i], &ip, &peer_port) != 0) {
            fprintf(stderr, "[Node] Bad peer spec (expected ip:port): %s\n", argv[i]);
            continue;
        }
        int fd = connect_to_server(ip, peer_port);
        if (fd < 0) {
            fprintf(stderr, "[Node] Could not connect to %s:%d\n", ip, peer_port);
            continue;
        }
        if (add_peer(fd) != 0) {
            client_disconnect(fd);
            continue;
        }
        char hello[64];
        int n = snprintf(hello, sizeof(hello), "HELLO from node %d\n", port);
        send_data(fd, hello, (size_t)n);
    }

    printf("[Node %d] Ready. Type a message to broadcast, Ctrl-D to quit.\n", port);

    /* 3. Event loop: listen socket, stdin, and all peers */
    int running = 1;
    while (running) {
        struct pollfd pfds[MAX_POLL];
        int nfds = 0;

        pfds[nfds].fd = server_fd;  pfds[nfds].events = POLLIN; nfds++;
        pfds[nfds].fd = 0;          pfds[nfds].events = POLLIN; nfds++;
        for (int i = 0; i < peer_count; i++) {
            pfds[nfds].fd = peers[i];
            pfds[nfds].events = POLLIN;
            nfds++;
        }

        if (poll(pfds, (nfds_t)nfds, -1) < 0) {
            perror("poll failed");
            break;
        }

        /* Peer sockets first (iterate backwards: remove_peer swaps from the end) */
        for (int i = peer_count - 1; i >= 0; i--) {
            short re = pfds[i + 2].revents;
            if (!(re & (POLLIN | POLLHUP | POLLERR))) {
                continue;
            }
            char buf[BUF_SIZE];
            ssize_t n = server_receive_from_peer(peers[i], buf, sizeof(buf) - 1);
            if (n <= 0) {
                printf("[Node %d] Peer disconnected\n", port);
                remove_peer(i);
            } else {
                buf[n] = '\0';
                printf("[Node %d] Received: %s", port, buf);
                if (buf[n - 1] != '\n') {
                    printf("\n");
                }
            }
        }

        /* New incoming peer */
        if (pfds[0].revents & POLLIN) {
            int fd = accept_peer_connection(server_fd);
            if (fd >= 0 && add_peer(fd) != 0) {
                server_close(fd);
            }
        }

        /* stdin */
        if (pfds[1].revents & (POLLIN | POLLHUP)) {
            char line[BUF_SIZE];
            if (fgets(line, sizeof(line), stdin) == NULL) {
                running = 0;
            } else {
                broadcast(line, strlen(line));
            }
        }
    }

    for (int i = 0; i < peer_count; i++) {
        server_close(peers[i]);
    }
    server_close(server_fd);
    printf("[Node %d] Shut down.\n", port);
    return EXIT_SUCCESS;
}