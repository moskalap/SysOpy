//
// Created by przemek on 15.04.17.
//
#include <string.h>
#include "stdio.h"

int main(){
    int BFR_LEN=250;
    FILE *executor;
    char buf[BFR_LEN];
    while(1){
        fgets(buf,BFR_LEN,stdin);
        executor=popen("./exec/executor", "w");
        fwrite(buf,sizeof(char),strlen(buf),executor);
        fclose(executor);

    }

}