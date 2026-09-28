#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// =========================
// Server methods
// =========================

int init_server(int port)
{
    printf("Initializing server on port %d...\n", port);

    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket failed");
        return -1;
    }

    // Allow reuse of the address/port
    int opt = 1;

    if (setsockopt(server_fd,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &opt,
                   sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    // 2. Configure server address
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 3. Bind socket to IP + port
    if (bind(server_fd,
             (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    // 4. Start listening
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    printf("Server listening on port %d...\n", port);

    return server_fd;
}


int accept_client(int server_fd)
{
    // Accept a connection

    int client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0) {
        perror("accept failed");
        return -1;
    }

    printf("Client connected!\n");

    return client_fd;
}


// =========================
// Client methods
// =========================

int connect_to_server(const char *server_ip, int port)
{
    // 1. Create socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        perror("socket failed");
        return -1;
    }

    // 2. Configure server address
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // Convert IP string to binary form
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        perror("invalid server IP");
        close(client_fd);
        return -1;
    }

    // 3. Connect to server
    if (connect(client_fd,
                (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0) {
        perror("connect failed");
        close(client_fd);
        return -1;
    }

    printf("Connected to server %s:%d\n", server_ip, port);

    return client_fd;
}


// =========================
// Data methods
// =========================

int send_data(int client_fd, const char *data, size_t data_len)
{
    ssize_t bytes_sent = send(client_fd, data, data_len, 0);

    if (bytes_sent < 0) {
        perror("send failed");
        return -1;
    }

    return (int)bytes_sent;
}


int receive_data(int client_fd, char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }

    ssize_t bytes_received = recv(
        client_fd,
        buffer,
        buffer_size - 1,
        0
    );

    if (bytes_received < 0) {
        perror("receive failed");
        return -1;
    }

    if (bytes_received == 0) {
        // Connection closed by the other side
        return 0;
    }

    buffer[bytes_received] = '\0';

    return (int)bytes_received;
}


// =========================
// Close connection
// =========================

int network_close(int fd)
{
    if (close(fd) < 0) {
        perror("close failed");
        return -1;
    }

    return 0;
}