#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "network.h"

int network_init_server(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket failed");
        return -1;
    }

    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(fd);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        close(fd);
        return -1;
    }

    if (listen(fd, 16) < 0) {
        perror("listen failed");
        close(fd);
        return -1;
    }
    return fd;
}

int network_accept_client(int server_fd)
{
    int fd;
    do {
        fd = accept(server_fd, NULL, NULL);
    } while (fd < 0 && errno == EINTR);

    if (fd < 0) {
        perror("accept failed");
        return -1;
    }
    return fd;
}

int network_connect(const char *ip, int port)
{
    if (ip == NULL) {
        return -1;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);

    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        fprintf(stderr, "invalid server IP: %s\n", ip);
        close(fd);
        return -1;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect failed");
        close(fd);
        return -1;
    }
    return fd;
}

ssize_t network_send(int fd, const char *data, size_t len)
{
    if (fd < 0 || data == NULL) {
        return -1;
    }

    size_t total = 0;
    while (total < len) {
        ssize_t n = send(fd, data + total, len - total, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("send failed");
            return -1;
        }
        total += (size_t)n;
    }
    return (ssize_t)total;
}

ssize_t network_recv(int fd, char *buf, size_t len)
{
    if (fd < 0 || buf == NULL || len == 0) {
        return -1;
    }

    ssize_t n;
    do {
        n = recv(fd, buf, len, 0);
    } while (n < 0 && errno == EINTR);

    if (n < 0) {
        perror("recv failed");
        return -1;
    }
    return n;
}

void network_close(int fd)
{
    if (fd >= 0) {
        close(fd);
    }
}