#pragma once
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

#ifndef API_H
#define API_H


struct server {
    char fifo_name[L_tmpnam + 1];
    char pipefd[1000][1000];
    int listenfd;
    struct sockaddr_in serv_addr, cli_addr;
    int isInit;
};

void(* callbacks[1024])(char* message);

int init();

char* sendReq(char* string);

int createUser(char* channel, int role);

int deleteUser(int id);

int post(int id, char* DM);

int readMessage(int id, char* mesaj);

int addCallback(int id,void (* message_callback)(char* message));

#endif