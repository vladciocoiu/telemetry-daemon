#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "daemon.h"


struct channel root;
userscnt = 0;

struct channel* get_child_by_name(struct channel* parent, char* name) {
    printf("a intrat in functie cu parent %s si child %s\n", parent->name, name);
    // find name in the list of children channels
    struct channel_list_node* node;
    for(node = parent->children; node != NULL; node = node->next) {
        if(strcmp(node->ch->name, name) == 0) return node->ch;
    }
    printf("no copil found\n");
    // otherwise create a new channel with the given name
    struct channel * new_channel = (struct channel *) malloc(sizeof(struct channel));
    strncpy(new_channel->name, name, 64);
    new_channel->message_id = 0;
    memset(new_channel->message, 0, sizeof(new_channel->message));
    new_channel->children = NULL;

    // create new list node with the newly created channel
    struct channel_list_node * new_node = (struct channel_list_node *) malloc(sizeof(struct channel_list_node));
    new_node->next = NULL;
    new_node->ch = new_channel;

    // add node to children
    if(parent->children == NULL)
        parent->children = new_node;
    else
    for(node = parent->children; node != NULL; node = node->next) {
        if(node->next == NULL) {
            node->next = new_node;
            break;
        }
    }
    // return the new channel
    return new_channel;
}

// returns the channel with the given path, or creates a new channel
struct channel* get_channel(char* path) {
    char path2[1024];
    strncpy(path2, path, 1024);
    char *token = strtok(path2, "/");
    struct channel * current_ch = &root;
    while (token != NULL) {
        current_ch = get_child_by_name(current_ch, token);
        token = strtok(NULL, "/");
    }

    return current_ch;
}

// creates a new user for the specified channel
int tlm_open(int type, char * channel_path, char * fd) {
    struct tlm  token;
    token.type = type;
    token.channel = get_channel(channel_path);
    strncpy(token.fd, fd, 32);
    userscnt++;
    tokens[userscnt] = token;
    return userscnt;
}

// replaces the callback function with the one provided by the user
int tlm_callback(int id) {
    tokens[id].callback = 1;
    return 0;
}


char * tlm_read(int id) {
    struct tlm* token = &tokens[id];
    // if the user does not have read permission, then return id of -1 and empty string
    if (token->type != TLM_SUBSCRIBER && token->type != TLM_BOTH) {
        return "\0";
    }

    // return the requested id and message
    return token->channel->message;
}

void replace_message(struct channel * ch, char * message) {
    // replace message in channel
    strncpy(ch->message, message, 256);
    ch->message_id++;
    printf("%s\n",ch -> name);
    // replace message in children channels
    for(struct channel_list_node * node = ch->children; node != NULL; node = node->next) {
        printf("parinte : %s copil : %s\n", ch->name,node->ch->name);
        replace_message(node->ch, message);
    }

}

int tlm_post(int id, char * message) {
    struct tlm* token = &tokens[id];
    // no write permissions
    if (token->type != TLM_PUBLISHER && token->type != TLM_BOTH) {
        printf("NOT ALLOWED\n");
        return -1;
    }

    // actually replace message
    replace_message(token->channel, message);

    // execute callback
    // if (token->message_callback) {
    //     token->message_callback(token, token->channel->message);
    // }
    return 0;
}


void tlm_close(int id) {
    tokens[id].fd[0] = '\0';
    tokens[id].channel = NULL;
    tokens[id].callback = 0;
    tokens[id].type = 0;
}

void cleanup(struct channel * ch) {
    while (ch->children != NULL) {
        struct channel_list_node * temp = ch->children;
        ch->children = ch->children->next;
        cleanup(temp->ch);
        free(temp);
    }
    ch->children = NULL;
}


// int main() {
//     root = ;
//     cleanup(root);
//     return 0;
// }