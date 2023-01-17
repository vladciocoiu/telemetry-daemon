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

#include "API.h"

struct server daemonsv;

int init(){
    daemonsv.listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    daemonsv.serv_addr.sin_family = AF_INET;
    daemonsv.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    daemonsv.serv_addr.sin_port = htons(5000);

    char fifo_name[L_tmpnam + 1];
    tmpnam(fifo_name);
    if (mkfifo(fifo_name, 0666) == -1)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int SendRequest(char* filename, char* message, int len)
{
    printf("sendRequest params: fd = %s, message = %s, len = %d\n", filename, message, len);
    int fd = open(filename, O_WRONLY);
    write(fd, message, len);
    return 0;
}

char* createUser(char* string){
    //request fd operatia rol mesaj
    if(!daemonsv.isInit)
        init();
    char* response;

    return response;
}

void* ListenToPipe(void *p)
{
   char* filename = p;
   char recvBuff[1025];
   memset(recvBuff, '0', sizeof(recvBuff));
   printf("Listen to pipe received value: %s\n", filename);
   int fd = open(filename, O_RDONLY);
   while(1)
   {
      size_t readCount = read(fd, recvBuff, sizeof(recvBuff));
      if(readCount == 0)
      {
         sleep(1);
         continue;
      }
      recvBuff[readCount] = 0;
      //parse request there()
      //and send the response()
      
      printf("got from pipe: %s\n", recvBuff);
   }
   pthread_exit(&fd);
}