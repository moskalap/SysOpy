//
// Created by przemek on 12.04.17.
//

#include "master.h"
#include <sys/types.h>
#include  <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    char PATH[50];
    char buff[50];

    int fd;
    dev_t dev;
    int   status;
    strcpy(PATH,argv[1]);
    unlink(PATH);
    if(argc<2) {
        printf("usage [FIFO_NAME] [R]\n");
        EXIT_FAILURE;
    }else{
        int R=atoi(argv[2]);
        int T[R][R];

        int fifo=mkfifo(PATH, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
        sleep(1);
        if(fifo == 0){
            fd=open(PATH, O_RDONLY);
            if(fd<0){
                fprintf(stderr, "error while opening fifo.\n");
            }
            else {
                while(read(fd,buff, 50)){
                    printf(buff);
                }
            }

        }else{
            fprintf(stderr, "error while creating fifo.\n");
            EXIT_FAILURE;
        }
    }

}
