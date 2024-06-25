#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/local_socket"
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_un server_address;
    char buffer[BUFFER_SIZE];

    //Create a socket
    client_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Set up server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_LOCAL;
    strncpy(server_address.sun_path, SOCKET_PATH, sizeof(server_address.sun_path) - 1);

    //Establish a connection with the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(sa_family_t) + strlen(server_address.sun_path)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

    while (1) {
        //Input a message from the user
        printf("Enter a message (type 'quit' to exit): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove the newline character from fgets

        //Send the message to the server
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        //Check if the message is "quit"
        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Sent quit message. Exiting...\n");
            break;  // Exit the loop
        }

        // Clear the buffer for the next iteration
        memset(buffer, 0, sizeof(buffer));
    }

    //Close the socket
    close(client_socket);

    return 0;
}
