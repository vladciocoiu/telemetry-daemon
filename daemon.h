#pragma once

#ifndef DAEMON_H
#define DAEMON_H

#define TLM_PUBLISHER 0x1
#define TLM_SUBSCRIBER 0x2
#define TLM_BOTH 0x3

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
    void (* message_callback)(struct tlm * token, char * message);
    char fd[32];
};

int tlm_open(int type, char * channel_path, char * fd);

int tlm_callback(struct tlm * token, void (* message_callback)(struct tlm * token, char * message));

const char * tlm_read(struct tlm * token, unsigned int * message_id);

int tlm_post(struct tlm * token, char * message);

void tlm_close(struct tlm * token);


#endif