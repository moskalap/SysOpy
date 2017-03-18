//
// Created by przemek on 16.03.17.
//


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <zconf.h>
typedef struct Option{
    char* fileName;
    char option;
    int recordSize;
    int records;
}Option;

static char * RANDOM = "/dev/urandom";
static char * OPEN_OPT = "ab+";
enum Arguments{
    file, version, record_size, file_size,
};


void generateRecords(char* outputFileName, int record_size, int records) {
    int output = open(outputFileName, O_CREAT|O_APPEND|O_WRONLY,  S_IRUSR | S_IRGRP | S_IROTH );
    int input = open(RANDOM, O_RDONLY);
    char byte;

    for (int record = 0; record < records; record++) {
        for (int b = 0; b < record_size; b++) {
            read(input, &byte, sizeof(byte));
            write(output, &byte, sizeof(byte));

        }
        write(output, &("\n"), sizeof(byte));

    }
    close(output);
    close(input);
}


void shuffle(FILE * file, int recordSize, int records){


}
char getRec1stByte(FILE * file, int record){

}

void swap(FILE  * file, int a, int b){

}


void generateFile(char* name, int recordSize, int records){

    FILE *output =fopen(name,"wb");
    char* record;

    for(int i = 0; i< records; i++){

        fwrite(record, sizeof(char), sizeof(record), output);
    }


}
enum Arguments descript (char * arg){
    if (strcmp( arg, "-file") == 0) return file;
    else
    if(strcmp( arg, "-version") == 0) return version;
    else
    if(strcmp(arg, "-fsize") == 0) return file_size;
    else
    if(strcmp(arg, "-rsize")==0) return record_size;



}
Option * parseOption(int a, char * argv[]){
    Option *option=malloc(sizeof(option));
    enum Arguments arg;
    for (int i=1; i<a; i++){//-file -u -fsize -rsize -
        arg = descript(argv[i]);
        switch(arg){
            case file:
                option->fileName=argv[i+1];
                break;
            case version:
                option->option=argv[i+1][0];
                break;
            case file_size:
                option->records=atoi(argv[i+1]);
                break;
            case record_size:
                option->recordSize = atoi(argv[i+1]);
                break;
            default:
                break;

        }
    }
    printf("Opcje:\n");
    printf("PLIK:");
    printf(option->fileName);
    printf("\nWERSJA: ");
    printf("%c", option->option);
    printf("\nREKORD: ");
    printf("%d", option->recordSize);

    printf("\nREKORDY: ");
    printf("%d", option->records);
    return option;

}

int main(int argc, char *argv[]){//-filename file -
    struct Option * options=parseOption(argc, argv);
    generateRecords(options->fileName, options->recordSize, options->records);






}
