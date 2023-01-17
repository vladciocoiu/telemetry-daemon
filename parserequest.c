#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseRequest(char *req, char *fd, char* op, char* ch_path, int* role, char* msg) {
    char *params[5];
    int size = 0;

    char *token = strtok(req, " ");
    while (token != NULL) {
        if(size >= 5) return -1;

        params[size] = token;
        size++;
        token = strtok(NULL, " ");
    }

    if(size < 4) return -1;

    strncpy(fd, params[0], 1024);
    strncpy(op, params[1], 1024);
    strncpy(ch_path, params[2], 1024);
    *role = atoi(params[3]);

    if(strcmp(op, "post") == 0) {
        if(size != 5) return -1;

        strncpy(msg, params[4], 1024);
    } else {
        msg = "\0";
    }

    return 0;
}