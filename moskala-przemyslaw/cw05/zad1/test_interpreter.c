//
// Created by przemek on 05.04.17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int BFFR_L=1024;
int MAX_ARGS=10;
int MAX_PIPES=25;
int run=1;

void interact(){
    char input[BFFR_L];

    while(run){
        fgets(input, BFFR_L, stdin);
        input[strlen(input)-1]='0';
    }


}

int main(int argc, char * argv[]){



interact();

}
