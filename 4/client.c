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

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    while (1) {
        printf("Client: ");
        fgets(buffer, MAX_MSG_SIZE, stdin);
        
        send(client_socket, buffer, strlen(buffer), 0);
        if (strncmp(buffer, "exit",4) == 0) {
            printf("Connection closed by client.\n");
            break;
        }
        bzero(buffer,1024);

        // Receive data from server
        recv(client_socket, buffer, MAX_MSG_SIZE, 0);
        if (strncmp(buffer, "exit",4) == 0) {
            printf("Connection closed by server.\n");
            break;
        }
        
        printf("Server: %s", buffer);
        bzero(buffer,1024);
     
    }

    close(client_socket);

    return 0;
}