#include <stdio.h>
#include <stdlib.h>

#include "network.h"

#define PORT 8080

int main(void)
{
    // Initialize the server
    int server_fd = init_server(PORT);

    if (server_fd < 0) {
        return EXIT_FAILURE;
    }

    // Wait for a client
    int client_fd = accept_client(server_fd);

    if (client_fd < 0) {
        network_close(server_fd);
        return EXIT_FAILURE;
    }

    // Receive data from client
    char buffer[1024];

    int bytes_received = receive_data(
        client_fd,
        buffer,
        sizeof(buffer)
    );

    if (bytes_received < 0) {
        network_close(client_fd);
        network_close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Received: %s\n", buffer);

    // Close connections
    network_close(client_fd);
    network_close(server_fd);

    return EXIT_SUCCESS;
}