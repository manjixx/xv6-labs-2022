#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc != 2){ // forgets passed an argument
        fprintf(2, "usage: sleep <time> \n");
        exit(1);
    }else{
        sleep(atoi(argv[1]));
        exit(0);
    }

}