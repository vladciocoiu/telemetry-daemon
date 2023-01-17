#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../parseresponse.h"
#include "API.h"

struct server daemonsv;

void* listenToPipe(void *p)
{
   char* filename = p;
   char recvBuff[1025];
    memset(recvBuff, '\0', sizeof(recvBuff));
   printf("Listen to pipe received value: %s\n", filename);
   int fd = open(filename, O_RDWR);
    printf("nu e nici de la open\n");
   while(1)
   {
      size_t readCount = read(fd, recvBuff, sizeof(recvBuff));
      printf("nu e nici de la read\n");
      if(readCount == 0)
      {
        printf("nimic\n");
         sleep(1);
         continue;
      }
      printf("recvbuff %s\n", recvBuff);
      recvBuff[readCount] = 0;
      //parse response here()
      struct response rsp;
      printf("ajunge\n");
      parseResponse(recvBuff,&rsp);
      printf("trece\n");
      printf("got from pipe: %s\n", recvBuff);
   }
   pthread_exit(&fd);
}

int init(){
    daemonsv.listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    daemonsv.serv_addr.sin_family = AF_INET;
    daemonsv.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    daemonsv.serv_addr.sin_port = htons(5000);

    tmpnam(daemonsv.fifo_name);
    if (mkfifo(daemonsv.fifo_name, 0666) == -1)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pthread_t threadListenToPipe;
    pthread_create(&threadListenToPipe, NULL, listenToPipe, daemonsv.fifo_name);
    
    return 0;
}

char* GetServerPipeFd(char* recvBuff)
{
    int len = sizeof(daemonsv.serv_addr);
    sendto(daemonsv.listenfd, "pipeFd", strlen("pipeFd"), 0, (const struct sockaddr *)&daemonsv.serv_addr, len);
    int nmb = recvfrom(daemonsv.listenfd, recvBuff, 1024, 0, (struct sockaddr *) &daemonsv.serv_addr, &len);
    recvBuff[nmb] = 0;
    if(fputs(recvBuff, stdout) == EOF)
    {
        printf("\n Error : Fputs error\n");
    }
    printf("!!! %s\n", recvBuff);
}

int sendRequest(char* filename, char* message, int len)
{
    printf("sendRequest params: fd = %s, message = %s, len = %d\n", filename, message, len);
    int fd = open(filename, O_WRONLY);
    write(fd, message, len);
    return 0;
}

//tlm_open
int createUser(char* channel, int role){ 
    //request fd operatia rol mesaj
    //fd registerUser channel role
    if(!daemonsv.isInit)
        init();
    char message[1024] = {'\0'};
    // memset('0', message, sizeof(message));
    snprintf(message, sizeof(message), "%s registerUser %s %d", daemonsv.fifo_name, channel, role );
    char fd[1024];
    GetServerPipeFd(fd);
    printf("fd!!! %s\n", fd);
    sendRequest(fd, message, strlen(message));
    return 0;
}

