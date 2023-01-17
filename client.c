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

char* GetServerPipeFd()
{
    int len = sizeof(serv_addr);
    sendto(sockfd, "pipeFd", strlen("pipeFd"), 0, (const struct sockaddr *)&serv_addr, len);
    int nmb = recvfrom(sockfd, recvBuff, sizeof(recvBuff)-1, 0, (struct sockaddr *) &serv_addr, &len);
    recvBuff[nmb] = 0;
    if(fputs(recvBuff, stdout) == EOF)
    {
        printf("\n Error : Fputs error\n");
    }
    return recvBuff;
}

int SendRequest(char* filename, char* message, int len)
{
    printf("sendRequest params: fd = %s, message = %s, len = %d\n", filename, message, len);
    int fd = open(filename, O_WRONLY);
    write(fd, message, len);
    return 0;
}


// int main(int argc, char *argv[])
// {
//     int n = 0;

//     if(argc != 2)
//     {
//         printf("\n Usage: %s <ip of server> \n",argv[0]);
//         return 1;
//     } 

//     memset(recvBuff, '0',sizeof(recvBuff));
//     if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//     {
//         printf("\n Error : Could not create socket \n");
//         return 1;
//     } 

//     memset(&serv_addr, '0', sizeof(serv_addr)); 

//    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    serv_addr.sin_port = htons(5000);


//     if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
//     {
//         printf("\n inet_pton error occured\n");
//         return 1;
//     } 


//     printf("sock fd = %d\n", sockfd);
//     printf("am ajuns aici\n");
    
//     char* pipeFd = GetServerPipeFd();
//     printf("pipeFd = %s\n", pipeFd);
//     while(1)
//     {
//         SendRequest(pipeFd, "request ceva ceva ceva", sizeof("request ceva ceva ceva"));
//         perror("SendRequest");
//         sleep(1);
//     }
    
    
    
//     if(n < 0)
//     {
//         printf("\n Read error \n");
//     } 

//     return 0;
// }