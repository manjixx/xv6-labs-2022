#include "Kernel/types.h"
#include "user/user.h"

# define RD 0     // pipe read ends
# define WR 1     // pipe write ends

int main(int argc, char *argv[]){

    char buf = 'P'; //transfer byte

    int fd_f2c[2]; // father -> child
    int fd_c2f[2]; // child -> father
    // fork()在父进程中返回子进程的PID,在子进程中返回0

    pipe(fd_f2c);
    pipe(fd_c2f);

    int pid = fork();
    int exit_status = 0;

    if(pid < 0) {
        fprintf(2, "fork() error!\n");

        close(fd_f2c[RD]);
        close(fd_f2c[WR]);
        close(fd_c2f[RD]);
        close(fd_c2f[WR]);
        // error exit
        exit(1);
    } else if(pid == 0) { // child

        // close the end of the father side
        close(fd_f2c[WR]);
        close(fd_c2f[RD]);
        
        // child read
        if(read(fd_f2c[RD], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "child read() error!\n");
            exit_status = 1;
        }else{
            fprintf(1, "%d: received ping\n", getpid());
        }

        // child write
        if(write(fd_c2f[WR], &buf,sizeof(char)) != sizeof(char)){
            fprintf(2, "child write() error!\n");
            exit_status = 1;
        }

        close(fd_f2c[RD]);
        close(fd_c2f[WR]);

        exit(exit_status);

    } else { // father
        // close the end of child side
        close(fd_f2c[RD]);
        close(fd_c2f[WR]);

         // father write
        if(write(fd_f2c[WR], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "father write() error!\n");
            exit_status = 1;
        }

        // father read
        if(read(fd_c2f[RD], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "father read() error!\n");
            exit_status = 1;
        }else{
            fprintf(1, "%d: received pong\n", getpid());
        }

        close(fd_f2c[WR]);
        close(fd_c2f[RD]);

        exit(exit_status);
    }
}