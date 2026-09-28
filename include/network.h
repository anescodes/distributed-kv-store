#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <sys/types.h>

int network_init_server(int port);
int network_accept_client(int server_fd);

int network_connect(const char *host, int port);

ssize_t network_send(int fd, const void *data, size_t size);
ssize_t network_recv(int fd, void *buffer, size_t size);

int network_close(int fd);

#endif