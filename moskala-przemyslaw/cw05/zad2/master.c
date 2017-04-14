//
// Created by przemek on 12.04.17.
//
int BFRL=50;


#include "master.h"
#include <sys/types.h>
#include  <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <fcntl.h>
int scale_x(double x, int R){ //x <- -2 1
                                //res = [0,R]
    int res;
    int ratio = 300/R;
    x=x+2; //x<-0,3
    x=x*100;
    x=x/300;
    x=x*R;
    return (int)x;

}
int scale_y(double y, int R){ //x <- -2 1
                                //res = [0,R]
    int res;
    int ratio = 200/R;
    y=y+1;//x<-0,2
    y=y*100;
    y=y/200;
    y = y*R;
    return (int) y;

}
void scale(char * buff, int R){
    //r -2 1
    //i -1 1
    int scaled_x, scaled_y;
    double x = atof(strtok(buff, " "));
    double y =atof( strtok(NULL, " "));
    int i = atoi(strtok(NULL, " "));
    scaled_x = scale_x(x,R);
    scaled_y = scale_y(y,R);
    printf("%f %f %i-> %d %d %d\n",x,y,i,scaled_x,scaled_y,i);

}

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
                   scale(buff,R);
                }
            }

        }else{
            fprintf(stderr, "error while creating fifo.\n");
            EXIT_FAILURE;
        }
    }

}
