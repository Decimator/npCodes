#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> // for close()
 
int main(int argc, char **argv) {
    int sockfd, n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Use SOCK_DGRAM for UDP

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        bzero(sendline, 100);
        bzero(recvline, 100);

        fgets(sendline, 100, stdin);

        sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        socklen_t len = sizeof(servaddr);
        recvfrom(sockfd, recvline, 100, 0, (struct sockaddr *)&servaddr, &len);

        printf("%s", recvline);
    }

    close(sockfd);

    return 0;
}