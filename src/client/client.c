#include <stdio.h>
#include "network.h"
#include "client.h"

int connect_to_server(const char *server_ip, int port)
{
    int fd = network_connect(server_ip, port);
    if (fd >= 0) {
        printf("[Client Module] Connected to server at %s:%d\n", server_ip, port);
    }
    return fd;
}

ssize_t send_data(int client_fd, const char *data, size_t data_len)
{
    return network_send(client_fd, data, data_len);
}

ssize_t client_receive_data(int client_fd, char *buf, size_t len)
{
    return network_recv(client_fd, buf, len);
}

void client_disconnect(int client_fd)
{
    network_close(client_fd);
}