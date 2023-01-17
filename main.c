#include "API/API.h"

int main(int argc, char** argv){
    createUser("/r", 3);
    while(1){
        sleep(1);
    }

    return 0;
}