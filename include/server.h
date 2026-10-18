#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include <sys/types.h>

/* Start listening on `port`. Returns listening fd or -1. */
int start_server_module(int port);

/* Accept an incoming peer. Returns peer fd or -1. */
int accept_peer_connection(int server_fd);

/* Read data from a connected peer. Returns bytes, 0 if closed, -1 on error. */
ssize_t server_receive_from_peer(int peer_fd, char *buf, size_t len);

/* Send data to a connected peer. Returns bytes sent or -1. */
ssize_t server_send_to_peer(int peer_fd, const char *data, size_t len);

/* Close a peer or the listening socket. */
void server_close(int fd);

#endif /* SERVER_H */