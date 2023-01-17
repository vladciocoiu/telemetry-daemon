#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parseresponse.h"
    // int success;

    // char op; 

    // char ch_path; 

    // int role;

    // char msg;  

int parseResponse(char *resp, struct response* ret) {
    char *params[5];
    int size = 0;

    char *token = strtok(resp, " ");
    while (token != NULL) {
        if(size >= 5) return -1;

        params[size] = token;
        size++;
        token = strtok(NULL, " ");
    }

    if(size < 2) return -1;

    strncpy(ret->op, params[0], 1024);
    ret->success = atoi(params[1]);

    // ret->op success
    if(strcmp(ret->op, "deleteUser") == 0 || strcmp(ret->op, "post") == 0) {
        if(size != 2) return -1;

        return 0;
    }

    // op success ret->msg
    if(strcmp(ret->op, "read") == 0) {
        if(size != 3) return -1;

        strncpy(ret->msg, params[2], 1024);

        return 0;
    }

    // ret->op success ret->ch_path role
    if(strcmp(ret->op, "registerUser") == 0) {
        if(size != 4) return -1;

        strncpy(ret->ch_path, params[2], 1024);
        ret->role = atoi(params[3]);

        return 0;
    }

    // ret->op success ret->ch_path role ret->msg
    if(strcmp(ret->op, "callback") == 0) {
        if(size != 5) return -1;

        strncpy(ret->ch_path, params[2], 1024);
        ret->role = atoi(params[3]);

        strncpy(ret->msg, params[4], 1024);
    }

    return -1;
}

// int main() {

//     char *fd = (char*) malloc(sizeof(char) * 1024);
//     char *op = (char*) malloc(sizeof(char) * 1024);
//     char *ret->ch_path = (char*) malloc(sizeof(char) * 1024);
//     char *msg = (char*) malloc(sizeof(char) * 1024);
//     int* role = (int*) malloc(sizeof(int));


//     char str[] = "temp-xyq45z-jeff registerUser /com/abcd 1";
//     parseRequest(str, fd, op, ch_path, role, msg);

//     if(strcmp(fd, "temp-xyq45z-jeff")) {
//         printf("Test 1 failed. Expected fd = 1temp-xyq45z-jeff, given %s\n", fd);
//     }
//     if(strcmp(op, "registerUser")) {
//         printf("Test 1 failed. Expected op = registerUser, given %s\n", op);
//     }
//     if(strcmp(ch_path, "/com/abcd")) {
//         printf("Test 1 failed. Expected ch_path = /com/abcd, given %s\n", ch_path);
//     }
//     if(*role != 1) {
//         printf("Test 1 failed. Expected role = 1, given %d\n", *role);
//     }
//     if(strcmp(msg, "\0")) {
//         printf("Test 2 failed. Expected msg = NULL, given %s\n", msg);
//     }

//     char str2[] = "abcd:''/-13 post /com/wtf 3 hello,world!";
//     parseRequest(str2, fd, op, ch_path, role, msg);

//     if(strcmp(fd, "abcd:''/-13")) {
//         printf("Test 2 failed. Expected abcd:''/-13, given %s\n", fd);
//     }
//     if(strcmp(op, "post")) {
//         printf("Test 2 failed. Expected op = post, given %s\n", op);
//     }
//     if(strcmp(ch_path, "/com/wtf")) {
//         printf("Test 2 failed. Expected ch_path = /com/wtf, given %s\n", ch_path);
//     }
//     if(*role != 3) {
//         printf("Test 2 failed. Expected role = 3, given %d\n", *role);
//     }
//     if(strcmp(msg, "hello,world!")) {
//         printf("Test 2 failed. Expected msg = hello,world!, given %s\n", msg);
//     }

//     char str3[] = "0 post lskfjds sdfkjlsdj";
//     if(parseRequest(str3, fd, op, ch_path, role, msg) != -1) {
//         printf("Test 3 failed. Expected parseRequest() to be -1, given 0\n");
//     }

//     printf("Tests finished\n");

//     free(fd);
//     return 0;
// }