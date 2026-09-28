#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main(void)
{
    // Connect to server
    int client_fd = connect_to_server(SERVER_IP, PORT);

    if (client_fd < 0) {
        return EXIT_FAILURE;
    }

    // Message to send
    const char *message = "Hello from client!";

    // Send message
    int bytes_sent = send_data(
        client_fd,
        message,
        strlen(message)
    );

    if (bytes_sent < 0) {
        network_close(client_fd);
        return EXIT_FAILURE;
    }

    printf("Sent: %s\n", message);

    // Close connection
    network_close(client_fd);

    return EXIT_SUCCESS;
}