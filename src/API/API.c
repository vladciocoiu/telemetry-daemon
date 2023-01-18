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
pthread_mutex_t reqLock;
struct response ret;

void* listenToPipe(void *p)
{
   char* filename = p;
   char recvBuff[1025];
    memset(recvBuff, '\0', sizeof(recvBuff));
   printf("Listen to pipe: %s\n", filename);
   int fd = open(filename, O_RDWR);
   while(1)
   {
      size_t readCount = read(fd, recvBuff, sizeof(recvBuff));
      if(readCount == 0)
      {
         sleep(1);
         continue;
      }
      recvBuff[readCount] = 0;
      //parse response here()
      struct response rsp;
      parseResponse(recvBuff, &rsp);
      if(strcmp(rsp.op, "call") != 0){
        ret.id = rsp.id;
        strncpy(ret.msg, rsp.msg, 1024);
        ret.success = rsp.success; 
        pthread_mutex_unlock(&reqLock);
      }
      else
      {
        ret.id = rsp.id;
        strncpy(ret.msg, rsp.msg, 1024);
        (*callbacks[ret.id])(ret.msg);
      }
      printf("Got from pipe: %s\n", recvBuff);
    printf("\n--------------------------------------------\n\n");
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

    if (pthread_mutex_init(&reqLock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    pthread_mutex_lock(&reqLock);

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
    printf("\n");
    // printf("!!! %s\n", recvBuff);
}

struct response sendRequest(char* filename, char* message, int len)
{
    printf("sendRequest params: fd = %s, message = %s, len = %d\n", filename, message, len);
    int fd = open(filename, O_WRONLY);
    write(fd, message, len);

    pthread_mutex_lock(&reqLock);

    return ret;
}

int createUser(char* channel, int role) { 
    //fd registerUser channel role
    if(!daemonsv.isInit) init();

    char message[1024] = {'\0'};

    snprintf(message, sizeof(message), "%s registerUser %s %d", daemonsv.fifo_name, channel, role );
    char fd[1024];
    GetServerPipeFd(fd);
    struct response ret = sendRequest(fd, message, strlen(message));
    return ret.id;
}

int deleteUser(int id){ 
    char message[1024] = {'\0'};
    snprintf(message, sizeof(message), "%s deleteUser %d", daemonsv.fifo_name, id);
    char fd[1024];
    GetServerPipeFd(fd);
    sendRequest(fd, message, strlen(message));
    return 0;
}

int post(int id, char* DM){
    char message[1024] = {'\0'};
    snprintf(message, sizeof(message), "%s post %d %s", daemonsv.fifo_name, id, DM);
    char fd[1024];
    GetServerPipeFd(fd);
    sendRequest(fd, message, strlen(message));
    return 0;
}

int readMessage(int id, char* mesaj){
    char message[1024] = {'\0'};
    snprintf(message, sizeof(message), "%s read %d", daemonsv.fifo_name, id);
    char fd[1024];
    GetServerPipeFd(fd);
    struct response ret = sendRequest(fd, message, strlen(message));
    strncpy(mesaj, ret.msg, 1024);
    return 0;
}

int addCallback(int id,void (* message_callback)(char* message)){
    char message[1024] = {'\0'};
    snprintf(message, sizeof(message), "%s callback %d", daemonsv.fifo_name, id);
    char fd[1024];
    callbacks[id] = message_callback;
    GetServerPipeFd(fd);
    sendRequest(fd, message, strlen(message));
    return 0;
}