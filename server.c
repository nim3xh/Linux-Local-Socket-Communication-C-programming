#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/local_socket"
#define LISTEN_BACKLOG 5
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_un server_address, client_address;
    socklen_t addr_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    //Create a socket
    server_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Set up server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_LOCAL;
    strncpy(server_address.sun_path, SOCKET_PATH, sizeof(server_address.sun_path) - 1);

    //Bind the socket to the address
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(sa_family_t) + strlen(server_address.sun_path)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    //Listen for incoming connections
    if (listen(server_socket, LISTEN_BACKLOG) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on local socket %s...\n", SOCKET_PATH);

    while (1) {
        //Accept connections in an event loop
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue; // Continue to the next iteration
        }

        printf("Connection accepted from client\n");

        //Read data from the socket and print to the console
        ssize_t bytes_received;
        while ((bytes_received = read(client_socket, buffer, sizeof(buffer))) > 0) {
            // Print the received data
            printf("Received data from client: %.*s\n", (int)bytes_received, buffer);

            // Check if the message is "quit"
            if (strncmp(buffer, "quit", 4) == 0) {
                printf("Received quit message. Exiting...\n");
                close(client_socket);
                close(server_socket);
                unlink(SOCKET_PATH);
                exit(EXIT_SUCCESS);
            }

            // Clear the buffer for the next iteration
            memset(buffer, 0, sizeof(buffer));
        }

        if (bytes_received == -1) {
            perror("Read error");
        }

        //Close the client socket
        close(client_socket);
    }

    // The server will not reach this point in this example

    return 0;
}
