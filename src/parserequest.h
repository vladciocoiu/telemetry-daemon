#pragma once

#ifndef PARSEREQUEST_H
#define PARSEREQUEST_H

struct request {
    char fd[1024];
    char op[1024];
    char ch_path[1024];
    int role;
    char msg[1024];
    int id;
};

int parseRequest(char *req, struct request  *req_);

#endif