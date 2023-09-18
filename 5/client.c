#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_MSG_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG_SIZE];

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
        printf("Client: ");
        fgets(buffer, MAX_MSG_SIZE, stdin);

        // Send a message to the server
        sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by client.\n");
            break;
        }

        bzero(buffer,1024);

        // Receive a message from the server
        ssize_t recv_len = recvfrom(client_socket, buffer, MAX_MSG_SIZE, 0, NULL, NULL);
        if (recv_len < 0) {
            perror("Error in receiving");
            exit(1);
        }

        //buffer[recv_len] = '\0'; // Null-terminate the received message

        printf("Server: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by server.\n");
            break;
        }

        bzero(buffer,1024);
    }

    close(client_socket);
    return 0;
}
