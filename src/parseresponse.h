#pragma once

#ifndef PARSERESPONSE_H
#define PARSERESPONSE_H

struct response {
    int success;
    char op[1024]; 
    char ch_path[1024]; 
    int role;
    char msg[1024];  
    int id;
};

int parseResponse(char *resp, struct response *rsp);

#endif