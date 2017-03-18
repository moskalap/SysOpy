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
    read(file, tmp, (size_t)(recordSize +1)); // znajdz 1 rekod
    strcpy(tmp,res);


    lseek(file, record2*(recordSize+1), SEEK_SET); //znajdz 2
    read(file, res,  recordSize+1);
    lseek(file, record1*(recordSize+1), SEEK_SET);

    write(file, res, (size_t)(recordSize+1));
    lseek(file, record2*(recordSize+1), SEEK_SET);//zapisz tmp ->re
    write(file,  tmp, (size_t)(recordSize+1));
    //fwrite(res, )


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
void shuffle(char * filename, int recordSize, int records, char version){
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
char getRec1stByte(FILE * file, int record){

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

/**
 * Sorting a file with records, according to the option:
 *
 * @param filename - name of file with records
 * @param records - amount of records in file
 * @param recordSize - size of single record
 * @param option  - 'l' --> using  library IO functions;
 *                  's' --> using systems IO functions;
 */
void sort (char * filename, int records, int recordSize, char option){
    FILE * file=fopen(filename, "r+");
    switch (option){

        case 'l':

            bubble_sort_l(file, records, recordSize);
            break;
        case 's':
            break;

    }
    fclose(file);
}
int main(int argc, char *argv[]){
    srand(time(NULL));
    //-filename file -
    struct Option * options=parseOption(argc, argv);
   // generateRecords("ex1.txt", 5, 10);
    FILE* file=fopen("ex1.txt", "r+");
    printf("\n%d", sizeof("\n"));
    sort("ex1.txt",10,5,'l' );





}
