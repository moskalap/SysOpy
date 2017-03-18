//
// Created by przemek on 16.03.17.
//
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <zconf.h>

#include <time.h>
enum Functions{shuffle, sort, generate};

typedef struct Option{
    char* fileName;
    char option;
    int recordSize;
    int records;
    char* function;
}Option;
static char * RANDOM = "/dev/urandom";

enum Arguments{
    file, version, record_size, file_size, function
};
/**
 * Generates a records
 * @param outputFileName
 * @param record_size
 * @param records
 */
void generate_records(char* outputFileName, int record_size, int records) {
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
 * Generates a permutation of records in file using libs
 * @param filename - file with records
 * @param recordSize - size of records in bytes
 * @param records - amount of records
 */
void shuffle_l(char * filename, int recordSize, int records){
    FILE *file =fopen(filename, "r+"); // read with update;

    int i, j;
    for( i = records-1; i>0; i--){
        j=rand()%(i+1);
        swap_l(file, i, j, recordSize);
    }
}
/**
 * Swap two element in files
 * @param file - file with records
 * @param recrod1 - index of record 1
 * @param record2 - index of record 2
 */
void swap_s(int file, int record1, int record2, int recordSize){

    char tmp[recordSize+sizeof(char)];
    char res[recordSize+sizeof(char)];

    lseek(file, record1*(recordSize+1), SEEK_SET);
    read(file, res, (size_t)(recordSize +1)); // znajdz 1 rekod
    strcpy(tmp,res);


    lseek(file, record2*(recordSize+1), SEEK_SET); //znajdz 2
    read(file, res,  recordSize+1);
    lseek(file, record1*(recordSize+1), SEEK_SET);

    write(file, res, (size_t)(recordSize+1));
    lseek(file, record2*(recordSize+1), SEEK_SET);//zapisz tmp ->re
    write(file,  tmp, (size_t)(recordSize+1));



}
/**
 * Generates a permutation of records in file usings systems
 * @param filename - file with records
 * @param recordSize - size of records in bytes
 * @param records - amount of records
 */
void shuffle_s(char * filename, int recordSize, int records){
    int file= open(filename, O_RDWR);

    int i, j;
    for( i = records-1; i>0; i--){
        j=rand()%(i+1);
        swap_s(file, i, j, recordSize);
    }
}
/*
 * Generates a permutation of records included in file
 * depending of version:
 * 's' --> using sys function
 * 'l' --> using lib function
 */
void shufflee(char * filename, int recordSize, int records, char version){
    switch(version){
        case 's':
            shuffle_s(filename, recordSize, records);
            break;
        case 'l':
            shuffle_l(filename, recordSize, records);
            break;
        default:
            printf("\nerrUnknown option!\n");
            break;

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
    else
    if(strcmp(arg, "-fun")==0) return function;



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
            case function:
                option->function=argv[i+1];
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
int compare_l( FILE * file, int index1, int index2, int recordSize){
    char record1[recordSize+sizeof(char)];
    char record2[recordSize+sizeof(char)];

    fseek(file, index1*(recordSize+1), 0); // znajdz 1 rekod
    fread(record1, sizeof(char),recordSize+1, file);
    fseek(file, index2*(recordSize+1), 0); // znajdz 1 rekod
    fread(record2, sizeof(char),recordSize+1, file);

    if(record1[0]<record2[0]) return -1;
    if(record1[0]==record2[0]) return 0;
    return 1;



}
int compare_s( int file, int index1, int index2, int recordSize){
    char record1[recordSize+sizeof(char)];
    char record2[recordSize+sizeof(char)];
    lseek(file, index1*(recordSize+1), SEEK_SET);
    read(file, record1, recordSize+1);
    lseek(file, index2*(recordSize+1), SEEK_SET);
    read(file, record2, recordSize+1);
    // fread(record1, sizeof(char),recordSize+1, file);

    if(record1[0]<record2[0]) return -1;
    if(record1[0]==record2[0]) return 0;
    return 1;



}
/**
 * Sorting using libs functions
 * @param file
 * @param records
 * @param recordSize
 */
void bubble_sort_l(FILE * file, int records, int recordSize){
    for(int i=0; i<records; i++)
        for(int j=0; j< records-i-1; j++){
            if(compare_l(file, j, j+1, recordSize) >0)
                swap_l(file, j, j+1, recordSize);
        }

}
void bubble_sort_s(int file, int records, int recordSize){
    for(int i=0; i<records; i++)
        for(int j=0; j< records-i-1; j++){
            if(compare_s(file, j, j+1, recordSize) >0)
                swap_s(file, j, j+1, recordSize);
        }

}
/**
 * Sorting a file with records, according to the option:
 *
 * @param filename - name of file with records
 * @param records - amount of records in file
 * @param recordSize - size of single record
 * @param option  - 'l' --> using  library IO functions;
 *                  's' --> using systems IO functions;
 */
void bubble_sort (char * filename, int records, int recordSize, char option){
    FILE * file=fopen(filename, "r+");
    int file1=open(filename, O_RDWR);
    switch (option){

        case 'l':

            bubble_sort_l(file, records, recordSize);
            break;
        case 's':
            bubble_sort_s(file1,records,recordSize);
            break;

    }
    fclose(file);
    close(file1);
}
int check_if_correct(Option* option){
    if(option->records<1) {
        printf("\nAmount of records can not be less than 1!\n")
        return -1;
    }
    if(option->recordSize<1){
        printf("\nRecord size can not be less than 1!\n")
        return -2;
    }
    if(!option->option == 'l' && !option->option == 's'){
        printf("\nVersion of program are:\ns-for system functions\nl - for library functions")
        return -2;
    }
    if(option->function == NULL){
        printf("\nFunctions of program are:\nshuffle, sort or generate");
        return -2;
    }
    return 0;


}


int main(int argc, char *argv[]){
    srand(time(NULL));
    //-filename file -
    struct Option * options=parseOption(argc, argv);
    if(check_if_correct(options)==0)
        switch(descript_function(options->function)){
            case generate:
                generate_records(options->fileName,options->recordSize,options->records);
                break;
            case shuffle:
                shufflee(options->fileName,options->recordSize, options->records,options->option);
                break;
            case sort:
                bubble_sort(options->fileName, options->records, options->recordSize, options->option)

                break;
            default:
                printf("\nUnknown function\nUse -fun {generate, shuffle, sort}\n");
        }








}
