#include "API/API.h"

void functie(char* message){
    printf("merge %s\n", message);
    
}

int main(int argc, char** argv){
    int 
    id = createUser("/r", 3), 
    id2 = createUser("/r/aaaaa", 3), 
    id3 = createUser("/r/aaaaa/asdas", 3);
    char mesajj[1024];
    post(id, "hello");
    post(id2, "world");
    addCallback(id, *functie);
    post(id, "corbu");
    readMessage(id3, mesajj);
    deleteUser(id);
    deleteUser(id2);
    deleteUser(id3);
    printf("mesajul este %s\n", mesajj);
    // post(id2, "djaslkdw");
    while(1){
        sleep(1);
    }

    return 0;
}