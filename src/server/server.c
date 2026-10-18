#include <stdio.h>
#include "network.h"
#include "server.h"

int start_server_module(int port)
{
    printf("[Server Module] Initializing server on port %d...\n", port);
    int fd = network_init_server(port);
    if (fd >= 0) {
        printf("[Server Module] Listening on port %d\n", port);
    }
    return fd;
}

int accept_peer_connection(int server_fd)
{
    int fd = network_accept_client(server_fd);
    if (fd >= 0) {
        printf("[Server Module] Peer connected successfully!\n");
    }
    return fd;
}

ssize_t server_receive_from_peer(int peer_fd, char *buf, size_t len)
{
    return network_recv(peer_fd, buf, len);
}

ssize_t server_send_to_peer(int peer_fd, const char *data, size_t len)
{
    return network_send(peer_fd, data, len);
}

void server_close(int fd)
{
    network_close(fd);
}