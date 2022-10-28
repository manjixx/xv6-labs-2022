#include "Kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

/**
 * 
 * p = get a number from left neighbor
 * print p
 * loop:
 *  n = get a number from left neighbor
 *  if (p does not divide n)
 *      send n to right neighbor
*/

/**
 * @brief:read the first data of left pipe and print
 * @param: left pipe
 * @param: address of first data  
 * @return: 0: have data, -1: no data
*/

int left_first_data(int left_pipe[2], int *dst){

    if(read(left_pipe[RD], dst, sizeof(int)) == INT_LEN){
        printf("prime %d\n", *dst);
        return 0;
    }else{
        return -1;
    }

}

/**
 * @brief: transmit data to right neighbour
 * @param: left_pipe
 * @param: the pipe connect left process and right process
 * @first: first data of left pipe
*/

void transmit_data(int left_pipe[2], int pipe[2], int first){
    int data;

    /*read data from left pipe*/
    while(read(left_pipe[RD], &data, sizeof(int)) == INT_LEN){
        /* indivisible numbers are written to the right pipe*/
        if(data % first){
            write(pipe[WR], &data, INT_LEN);
        }
    }
    close(left_pipe[RD]);
    close(pipe[WR]);
}


/**
 * @brief: find primes
 * @param: left pipe
*/
void primes(int left_pipe[2]){
    
    /*close left pipe*/
    close(left_pipe[WR]);

    int first;

    /* read frist data of left pipe*/
    if(left_first_data(left_pipe, &first) == 0){
        /* create new pipe to connect this process and child process*/
        int p[2];
        pipe(p);
        transmit_data(left_pipe, p, first);

        /* create child process */
        if(fork() == 0){
            primes(p);
        }else{
            close(p[RD]);
            wait(0);
        }   
    }
    exit(0);
}

int main(int argc, char *argv[]){

    /* create first process read data */
    int p[2];

    pipe(p);

    for(int i = 2;i <= 35; i++){
        write(p[WR], &i, INT_LEN);
    }

    if(fork() == 0){
        primes(p);
    }else{
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}