#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


/**
 * @brief:find by path and filename
 * @param: path, file path
 * @param: filename
*/
void find(char *path, const char *filename){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    /*open path*/
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    
    /*put fd state into stat*/
    if(fstat(fd, &st) < 0){
        fprintf(2,"find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    /* the first argc type must be directory*/
    if(st.type !=  T_DIR){
        fprintf(2, "usage: find <DIRECTORY> <filename>\n");
        return;
    }

    /*path length to long*/
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }
        /* add path name*/
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0; // end of string

        /* put buf stat into st*/
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        /*don't recurse into "." and "..".*/
        if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0){
            find(buf, filename);
        }else if (strcmp(filename, p) == 0)
            printf("%s\n", buf);
    }
    close(fd);
}

int main(int argc, char * argv[]){

    if(argc != 3){
        fprintf(2, "usage: find <directory> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}