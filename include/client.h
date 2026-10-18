#ifndef CLIENT_H
#define CLIENT_H

#include <stddef.h>
#include <sys/types.h>

/* Connect to a peer server. Returns fd or -1. */
int connect_to_server(const char *server_ip, int port);

/* Send data to the connected server. Returns bytes sent or -1. */
ssize_t send_data(int client_fd, const char *data, size_t data_len);

/* Receive data from the connected server. Returns bytes, 0 if closed, -1 on error. */
ssize_t client_receive_data(int client_fd, char *buf, size_t len);

/* Close the client connection. */
void client_disconnect(int client_fd);

#endif /* CLIENT_H */

