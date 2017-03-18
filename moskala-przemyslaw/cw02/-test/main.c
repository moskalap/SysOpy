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
    int output = open(outputFileName, O_CREAT|O_APPEND|O_WRONLY,  S_IRUSR |S_IWUSR|  S_IRGRP | S_IROTH | S_IWOTH |S_IXOTH );
    int input = open(RANDOM, O_RDONLY);
    char byte;

    for (int record = 0; record < records; record++) {
        for (int b = 0; b < record_size; b++) {
            read(input, &byte, sizeof(byte));
            write(output, &byte, sizeof(byte));

        }
        byte='\n';
        write(output, &byte, sizeof(byte));

    }
    close(output);
    close(input);
}
/**
 * Swap two element in files
 * @param file - file with records
 * @param recrod1 - index of record 1
 * @param record2 - index of record 2
 */
void swap_l(FILE * file, int recrod1, int record2, int recordSize){
    char tmp[recordSize+sizeof(char)];
    char res[recordSize+sizeof(char)];
    FILE* filec;
    fseek(file, recrod1*(recordSize+1), 0); // znajdz 1 rekod
    fread(res, sizeof(char),recordSize+1, file);  //zapisz do res
    strcpy(tmp,res);


    fseek(file, record2*(recordSize+1), 0); //znajdz 2
    fread(res, sizeof(char),  recordSize+1, file);
    fseek(file, recrod1*(recordSize+1), 0);

    fputs(res, file);
    fseek(file, record2*(recordSize+1), 0);//zapisz tmp ->re
    fputs(tmp,  file);
    //fwrite(res, )


}
/**
 * Generates a permutation of records in file
 * @param filename - file with records
 * @param recordSize - size of records in bytes
 * @param records - amount of records
 */
void shuffle_l(char * filename, int recordSize, int records){
    FILE *file =fopen(filename, "r+"); // read with update;

    int i, j;
    for( i = records-1; i>0; i--){
        j=rand()%(i+1);
    }
}
/*
static int rand_int(int n) {
    int limit = RAND_MAX - RAND_MAX % n;
    int rnd;

    do {
        rnd = rand();
    } while (rnd >= limit);
    return rnd % n;
}

void shuffle(int *array, int n) {
    int i, j, tmp;

    for (i = n - 1; i > 0; i--) {
        j = rand_int(i + 1);
        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}
*/


/*
 * Generates a permutation of records included in file
 * depending of version:
 * 's' --> using sys function
 * 'l' --> using lib function
 */
void shuffle(char * filename, int recordSize, int records, char version){
    switch(version){
        case 's':
            // shuffle_s(filename, recordSize, records);
            break;
        case 'l':
            shuffle_l(filename, recordSize, records);
            break;
        default:
            printf("\nerrUnknown option!\n");
            break;

    }



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
    //generateRecords("ex1.txt", 5, 10);
    FILE* file=fopen("ex1.txt", "r+");
    printf("\n%d", sizeof("\n"));
    swap_l(file, 2, 3 ,5);




}
