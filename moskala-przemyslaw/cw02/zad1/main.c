//
// Created by przemek on 16.03.17.
//
static char * RANDOM = "/dev/random";
static char * OPEN_OPT = "ab+";

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

char* generate_record(int record_size){
    char* record= malloc(sizeof(char));



}

void generate_file(char* name, int record_size, int records){
    int file=open(RANDOM, O_RDONLY);
    FILE *output =fopen(name,"wb");
    char* record = generate_record(record_size);
    fwrite(record, sizeof(char), sizeof(record), output);


    for(int i = 0; i< records; i++){

    }


}


int main(int argc, char *argv[]){

}