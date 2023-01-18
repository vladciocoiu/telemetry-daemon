#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <fcntl.h>

int sockfd;

char recvBuff[1024];
struct sockaddr_in serv_addr; 

int SendRequest(char* filename, char* message, int len) {
    printf("sendRequest params: fd = %s, message = %s, len = %d\n", filename, message, len);
    
    int fd = open(filename, O_WRONLY);
    write(fd, message, len);
    close(fd);

    return 0;
}