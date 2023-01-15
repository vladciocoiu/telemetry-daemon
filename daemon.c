#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


struct channel {
    char* name;
    unsigned int message_id;
    char message[256];
    struct channel_list_node *children;
};

struct channel_list_node {
    struct channel *ch;
    struct channel_list_node *next;
};

struct channel * root;

struct channel* get_child_by_name(struct channel* parent, char* name) {

    // find name in the list of children channels
    struct channel_list_node* node;
    for(node = parent->children; node != NULL; node = node->next) {
        if(node->ch->name == name) return node->ch;
    }

    // otherwise create a new channel with the given name
    struct channel * new_channel = (struct channel *) malloc(sizeof(struct channel));
    new_channel->name = name;
    memset(new_channel->message, 0, sizeof(new_channel->message));
    new_channel->children = NULL;

    // create new list node with the newly created channel
    struct channel_list_node * new_node = (struct channel_list_node *) malloc(sizeof(struct channel_list_node));
    new_node->next = NULL;
    new_node->ch = new_channel;

    // add node to children
    for(node = parent->children; node != NULL; node = node->next) {
        if(node->next == NULL) {
            node->next = new_node;
            break;
        }
    }

    // return the new node
    return new_node;
}

// returns the channel with the given path, or creates a new channel
struct channel* get_channel(char* path) {
    char *token = strtok(path, "/");

    struct channel * current_ch = root;
    while (token != NULL) {
        current_ch = get_child_by_name(current_ch, token);
        token = strtok(NULL, "/");
    }

    return current_ch;
}




int main() {
    root = (struct channel *) malloc(sizeof(struct channel));
    return 0;
}