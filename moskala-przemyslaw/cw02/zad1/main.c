//
// Created by przemek on 16.03.17.
//


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

static char * RANDOM = "/dev/random";
static char * OPEN_OPT = "ab+";
enum Arguments{
    file, version, record_size, file_size,
};


char* generateRecord(int record_size){
    FILE *input = fopen(RANDOM, "rb");
    char* record= (char*) malloc(sizeof(char)*record_size); //albo -1? ->sprawdzić
    fread(record, record_size, 1, input);
    fclose(input);
    return record;



}

void shuffle(FILE * file, int recordSize, int records){

}
char getRec1stByte(FILE * file, int record){

}

void swap(FILE  * file, int a, int b){ /

}

void bubbleSort(FILE * file, int recordSize, int records, char option){
    char a;
    char b;

    switch(option){
        case 'l':
            for(int i= 0; i<records; i++)
                for (int j=0; j<records-i-1; j++){

                    if(getRecord_lib(file,j)>getRecord_lib(file,j+1))
                        swap(file,j,j+1);

                }
            break;
        case 's':
            for(int i= 0; i<records; i++)
                for (int j=0; j<records-i-1; j++){
                    if(getRecord_sys(file,j)>getRecord_sys(file,j))
                        swap(file, j, j+1);



                }

            break
        default:
            break;

    }


}



void generateFile(char* name, int recordSize, int records){

    FILE *output =fopen(name,"wb");
    char* record;

    for(int i = 0; i< records; i++){
        record=generateRecord(recordSize);
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
                if(strcmp(arg, "-rsize")) return record_size;



}

int main(int argc, char *argv[]){//-filename file -
    enum Arguments arg;

    for (int i=1; i<argc; i++){//-file -u -fsize -rsize -
        arg = descript(argv[i]);
        switch(arg){
            case file:
                break;
            case version:
                break;
            case file_size:
                break;
            case record_size:
                break;
            default:
                break;

        }


    }


}