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
#include <sys/stat.h>
#include <fcntl.h>
#include "parserequest.h"
#include "daemon.h"

int listenfd = 0, connfd = 0;
struct sockaddr_in serv_addr, cli_addr;

void* SendFd(void* p)
{
   char* fName = p;
   printf("Received value: %s", fName);

   char recvBuff[1025];
   char sendBuff[1025];

   memset(sendBuff, '0', sizeof(sendBuff));
   memset(&serv_addr, '0', sizeof(serv_addr));
   
   snprintf(sendBuff, sizeof(sendBuff), "%s", fName);
   printf("sending %s", sendBuff);
   int len = sizeof(cli_addr);
      
   
   while (1)
   {
      int nrOfBytesReceived = recvfrom(listenfd, recvBuff, sizeof(recvBuff)-1, 0, (struct sockaddr *) &cli_addr, &len);
      recvBuff[nrOfBytesReceived] = 0;
      if(strcmp(recvBuff, "pipeFd") == 0)
      {
         printf("bits sent %d bytes to file descriptor %d\n",sendto(listenfd, sendBuff, strlen(sendBuff), 0, (const struct sockaddr *)&cli_addr, len), listenfd );
         perror("Error");
      }
      sleep(1);
   }
   // Return reference to global variable:
   pthread_exit(0);
}

void callUsers(struct channel * ch, char * message) {
    // replace message in channel
   for(int i = 1; i <= userscnt; i++){
        if(tokens[i].channel == ch && tokens[i].callback == 1){

         snprintf(message, sizeof(message), "call %d %s", i, message);
         SendResponse(tokens[i].fd, message, strlen(message));
        }
   }
            
    

    // replace message in children channels
    for(struct channel_list_node * node = ch->children; node != NULL; node = node->next) {
        
        callUsers(node->ch, message);
    }

}


int SendResponse(char* filename, char* message, int len)
{
    printf("sendResponse params: fd = %s, message = %s, len = %d\n", filename, message, len);
    int fd = open(filename, O_WRONLY);
    write(fd, message, len);
    return 0;
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
      struct request req;
      parseRequest(recvBuff,&req);
      if(strcmp(req.op, "registerUser") == 0){
         int id;
         char message[1024];
         id = tlm_open(req.role, req.ch_path, req.fd);
         snprintf(message, sizeof(message), "%s 1 %d", req.op, id);
         SendResponse(req.fd, message, strlen(message));
      }

      if(strcmp(req.op, "deleteUser") == 0){
         char message[1024];
         tlm_close(req.id);
         snprintf(message, sizeof(message), "%s 1", req.op);
         SendResponse(req.fd, message, strlen(message));
      }

      if(strcmp(req.op, "post") == 0){
         char message[1024];
         tlm_post(req.id, req.msg);
         snprintf(message, sizeof(message), "%s 1", req.op);
         SendResponse(req.fd, message, strlen(message));
         callUsers(tokens[req.id].channel, req.msg);
      }

      if(strcmp(req.op, "read") == 0){
         char message[1024];
         char* mesaj = tlm_read(req.id);
         snprintf(message, sizeof(message), "%s 1 %s", req.op, mesaj);
         SendResponse(req.fd, message, strlen(message));
      }

      if(strcmp(req.op, "callback") == 0){
         char message[1024];
         tlm_callback(req.id);
         snprintf(message, sizeof(message), "%s 1", req.op);
         SendResponse(req.fd, message, strlen(message));
      }
      printf("got from pipe: %s\n", recvBuff);
   }
   pthread_exit(&fd);
}

int main(int argc, char *argv[])
{

   
   time_t ticks;

   listenfd = socket(AF_INET, SOCK_DGRAM, 0);

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(5000);

   bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

   int pipefd[2];
   char fifo_name[L_tmpnam + 1];
   tmpnam(fifo_name);
   if (mkfifo(fifo_name, 0666) == -1)
   {
      perror("mkfifo");
      exit(EXIT_FAILURE);
   }
   //close(pipefd[1]);
   pthread_t threadSendFd;
   pthread_create(&threadSendFd, NULL, SendFd, fifo_name);
   
   pthread_t threadListenToPipe;
   pthread_create(&threadListenToPipe, NULL, ListenToPipe, fifo_name);


   printf("sended\n");

   pthread_join(threadSendFd, NULL);
   sleep(1);
}