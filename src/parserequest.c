#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parserequest.h"

//fd post userid message
int parseRequest(char *req, struct request* req_) {
    char params[5][1024];
    int size = 0;
    char req2[1024];

    strncpy(req2, req, 1024);
    char *token = strtok(req2, " ");

    while (token != NULL) {
        if(size >= 5) return -1;

        strncpy(params[size], token, 1024);
        size++;
        token = strtok(NULL, " ");
    }

    strncpy(req_->fd, params[0], 1024);
    strncpy(req_->op, params[1], 1024);

    //fd registeruser channel role
    if(strcmp(req_->op, "registerUser") == 0) {
        strncpy(req_ -> ch_path, params[2], 1024);
        req_->role = atoi(params[3]);
    }

    //fd post id message
    if(strcmp(req_->op, "post") == 0) {
        if(size != 4) return -1;
        req_->id = atoi(params[2]);
        strncpy(req_->msg, params[3], 1024);
    } 

    //fd read id
    if(strcmp(req_->op, "read") == 0) {
        if(size != 3) return -1;
        req_->id = atoi(params[2]);
    }

    //fd callback id
    if(strcmp(req_->op, "callback") == 0) {
        if(size != 3) return -1;
        req_->id = atoi(params[2]);
    }

    //fd deleteuser id
    if(strcmp(req_->op, "deleteUser") == 0) {
        if(size != 3) return -1;
        req_->id = atoi(params[2]);
    }
    return 0;
}