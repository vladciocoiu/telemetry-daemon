#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parseresponse.h"

int parseResponse(char *resp, struct response* ret) {
    char params[5][1024];
    int size = 0;
    char resp2[1024];
    strncpy(resp2, resp, 1024);
    char *token = strtok(resp2, " ");
    while (token != NULL) {
        if(size >= 5) return -1;

        strncpy(params[size], token, 1024);
        size++;
        token = strtok(NULL, " ");
    }
    if(size < 2) return -1;

    strncpy(ret->op, params[0], 1024);
    ret->success = atoi(params[1]);

    // registerUser success id
    if(strcmp(ret->op, "registerUser") == 0) {
        if(size != 3) return -1;

        ret->id = atoi(params[2]);

        return 0;
    }
    // op success
    if(strcmp(ret->op, "deleteUser") == 0 || strcmp(ret->op, "post") == 0) {
        if(size != 2) return -1;

        return 0;
    }

    // read success ret->msg
    if(strcmp(ret->op, "read") == 0) {
        if(size != 3) return -1;

        strncpy(ret->msg, params[2], 1024);

        return 0;
    }

    // callback success userid message
    if(strcmp(ret->op, "callback") == 0) {
        if(size != 4) return -1;
        ret->id = atoi(params[2]);
        strncpy(ret->msg, params[3], 1024);
    }

    return -1;
}