#pragma once

#ifndef DAEMON_H
#define DAEMON_H

#define TLM_PUBLISHER 0x1
#define TLM_SUBSCRIBER 0x2
#define TLM_BOTH 0x3

int userscnt;

struct channel {
    char name[64];
    unsigned int message_id;
    char message[256];
    struct channel_list_node *children;
};

struct channel_list_node {
    struct channel *ch;
    struct channel_list_node *next;
};

struct tlm {
    int type;
    struct channel * channel;
    int callback;
    char fd[32];
};
struct tlm tokens[4096];


int tlm_open(int type, char * channel_path, char * fd);

int tlm_callback(int id);

char * tlm_read(int id);

int tlm_post(int id, char * message);

void tlm_close(int id);

#endif