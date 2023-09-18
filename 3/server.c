#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char str[100];
    int socket_fd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // Use SOCK_DGRAM for UDP

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(22000);

    bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1) {
        bzero(str, 100);

        recvfrom(socket_fd, str, 100, 0, (struct sockaddr *)&cliaddr, &len);

        printf("Echoing back - %s", str);

        sendto(socket_fd, str, strlen(str), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(socket_fd);

    return 0;
}