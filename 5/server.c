#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_MSG_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[MAX_MSG_SIZE];

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Server is listening...\n");

    while (1) {
        // Receive a message from the client
        ssize_t recv_len = recvfrom(server_socket, buffer, MAX_MSG_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);
        if (recv_len < 0) {
            perror("Error in receiving");
            exit(1);
        }

       // buffer[recv_len] = '\0'; // Null-terminate the received message

        printf("Client: %s", buffer);
      

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by client.\n");
            break;
        }

        bzero(buffer,1024);

        // Send the received message back to the client (echo)
        printf("Server: ");
        fgets(buffer, MAX_MSG_SIZE, stdin);
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_size);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Chat ended by server.\n");
            break;
        }

        bzero(buffer,1024);
    }

    close(server_socket);
    return 0;
}
