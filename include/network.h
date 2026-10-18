#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <sys/types.h>

/* Create a listening TCP socket on `port`. Returns fd or -1. */
int network_init_server(int port);

/* Accept one pending connection on `server_fd`. Returns client fd or -1. */
int network_accept_client(int server_fd);

/* Connect to ip:port. Returns connected fd or -1. */
int network_connect(const char *ip, int port);

/* Send exactly `len` bytes (loops on partial sends). Returns bytes sent or -1. */
ssize_t network_send(int fd, const char *data, size_t len);

/* Receive up to `len` bytes. Returns bytes read, 0 on peer close, -1 on error. */
ssize_t network_recv(int fd, char *buf, size_t len);

/* Close a socket (safe on negative fds). */
void network_close(int fd);

#endif /* NETWORK_H */