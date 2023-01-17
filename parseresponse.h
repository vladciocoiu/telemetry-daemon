#pragma once

#ifndef PARSERESPONSE_H
#define PARSERESPONSE_H

struct response{
    int success;
    char op; 
    char ch_path; 
    int role;
    char msg;  
};

int parseResponse(char *resp, struct response *rsp);

#endif