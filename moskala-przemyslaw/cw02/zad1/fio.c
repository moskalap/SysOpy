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

enum Functions {
    shuffle, sort, generate
};

typedef struct Option {
    char *fileName;
    char option;
    int recordSize;
    int records;
    enum Functions function;
} Option;
static char *RANDOM = "/dev/urandom";

enum Arguments {
    file, version, record_size, file_size, function, nothing
};

/**
 * Generates a records
 * @param outputFileName
 * @param record_size
 * @param records
 */
void generate_records(char *outputFileName, int record_size, int records) {
    int output = open(outputFileName, O_CREAT | O_APPEND | O_WRONLY,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWOTH );
    int input = open(RANDOM, O_RDONLY);
    char byte;
    int record, b;
    for (record = 0; record < records; record++) {
        for (b = 0; b < record_size; b++) {
            read(input, &byte, sizeof(byte));
            write(output, &byte, sizeof(byte));

        }
        byte = '\n';
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
void swap_l(FILE *file, int recrod1, int record2, int recordSize) {
    char tmp[recordSize + sizeof(char)];
    char res[recordSize + sizeof(char)];

    fseek(file, recrod1 * (recordSize + 1), 0); // znajdz 1 rekod
    fread(res, sizeof(char), recordSize + 1, file);  //zapisz do res
    strcpy(tmp, res);

    fseek(file, record2 * (recordSize + 1), 0); //znajdz 2
    fread(res, sizeof(char), recordSize + 1, file);
    fseek(file, recrod1 * (recordSize + 1), 0);

    fputs(res, file);
    fseek(file, record2 * (recordSize + 1), 0);//zapisz tmp ->re
    fputs(tmp, file);
}

/**
 * Generates a permutation of records in file using libs
 * @param filename - file with records
 * @param recordSize - size of records in bytes
 * @param records - amount of records
 */
void shuffle_l(char *filename, int recordSize, int records) {
    FILE *file = fopen(filename, "r+"); // read with update;

    int i, j;
    for (i = records - 1; i > 0; i--) {
        j = rand() % (i + 1);
        swap_l(file, i, j, recordSize);
    }
    fclose(file);
}

/**
 * Swap two element in files using system functions
 * @param file - file with records
 * @param recrod1 - index of record 1
 * @param record2 - index of record 2
 */
void swap_s(int file, int record1, int record2, int recordSize) {

    char tmp[recordSize + sizeof(char)];
    char res[recordSize + sizeof(char)];

    lseek(file, record1 * (recordSize + 1), SEEK_SET);
    read(file, res, (size_t) (recordSize + 1)); // znajdz 1 rekod
    strcpy(tmp, res);
    lseek(file, record2 * (recordSize + 1), SEEK_SET); //znajdz 2
    read(file, res, recordSize + 1);
    lseek(file, record1 * (recordSize + 1), SEEK_SET);
    write(file, res, (size_t) (recordSize + 1));
    lseek(file, record2 * (recordSize + 1), SEEK_SET);//zapisz tmp ->re
    write(file, tmp, (size_t) (recordSize + 1));

}

/**
 * Generates a permutation of records in file usings systems
 * @param filename - file with records
 * @param recordSize - size of records in bytes
 * @param records - amount of records
 */
void shuffle_s(char *filename, int recordSize, int records) {
    int file = open(filename, O_RDWR);

    int i, j;
    for (i = records - 1; i > 0; i--) {
        j = rand() % (i + 1);
        swap_s(file, i, j, recordSize);
    }
}

/**
 * Generates a permutation of records included in file
 * depending of version:
 * 's' --> using sys function
 * 'l' --> using lib function
 */
void shufflee(char *filename, int recordSize, int records, char version) {
    switch (version) {
        case 's':
            shuffle_s(filename, recordSize, records);
            break;
        case 'l':
            shuffle_l(filename, recordSize, records);
            break;
        default:
            printf("\nerrUnknown version!\n");
            break;

    }


}

/**
 * descript argument by string
 * @param arg string
 * @return enum ARG
 */
enum Arguments descript(char *arg) {
    if (strcmp(arg, "-file") == 0) return file;
    else if (strcmp(arg, "-version") == 0) return version;
    else if (strcmp(arg, "-fsize") == 0) return file_size;
    else if (strcmp(arg, "-rsize") == 0) return record_size;
    else if (strcmp(arg, "-fun") == 0) return function;
    else return nothing;


}

/**
 * Compare first bytes of two records using lib functions
 * @param file
 * @param index1 of 1st record
 * @param index2 of 2nd record
 * @param recordSize in bytes
 * @return -1 if record1<record2
 *          0 if record1 == record 2
 *          if record1 => reocord 2
 */
int compare_l(FILE *file, int index1, int index2, int recordSize) {
    char record1[recordSize + sizeof(char)];
    char record2[recordSize + sizeof(char)];

    fseek(file, index1 * (recordSize + 1), 0); // znajdz 1 rekod
    fread(record1, sizeof(char), recordSize + 1, file);
    fseek(file, index2 * (recordSize + 1), 0); // znajdz 1 rekod
    fread(record2, sizeof(char), recordSize + 1, file);

    if (record1[0] < record2[0]) return -1;
    if (record1[0] == record2[0]) return 0;
    return 1;


}

/**
 * Compare first bytes of two records using sys functions
 * @param file
 * @param index1 of 1st record
 * @param index2 of 2nd record
 * @param recordSize in bytes
 * @return -1 if record1<record2
 *          0 if record1 == record 2
 *          if record1 => reocord 2
 */
int compare_s(int file, int index1, int index2, int recordSize) {
    char record1[recordSize + sizeof(char)];
    char record2[recordSize + sizeof(char)];
    lseek(file, index1 * (recordSize + 1), SEEK_SET);
    read(file, record1, recordSize + 1);
    lseek(file, index2 * (recordSize + 1), SEEK_SET);
    read(file, record2, recordSize + 1);
    // fread(record1, sizeof(char),recordSize+1, file);

    if (record1[0] < record2[0]) return -1;
    if (record1[0] == record2[0]) return 0;
    return 1;


}

/**
 * Sorting using libs functions
 * @param file
 * @param records
 * @param recordSize
 */
void bubble_sort_l(FILE *file, int records, int recordSize) {
    int i,j;
    for (i = 0; i < records; i++)
        for (j = 0; j < records - i - 1; j++) {
            if (compare_l(file, j, j + 1, recordSize) > 0)
                swap_l(file, j, j + 1, recordSize);
        }

}

/**
 * Sorting using sys functions
 * @param file
 * @param records
 * @param recordSize
 */
void bubble_sort_s(int file, int records, int recordSize) {
    int i,j;
    for (i = 0; i < records; i++)
        for (j = 0; j < records - i - 1; j++) {
            if (compare_s(file, j, j + 1, recordSize) > 0)
                swap_s(file, j, j + 1, recordSize);
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
void bubble_sort(char *filename, int records, int recordSize, char option) {
    FILE *file = fopen(filename, "r+");
    int file1 = open(filename, O_RDWR);
    switch (option) {

        case 'l':

            bubble_sort_l(file, records, recordSize);
            break;
        case 's':
            bubble_sort_s(file1, records, recordSize);
            break;

    }
    fclose(file);
    close(file1);
}

/**
 * checks if options are syntax correct
 * @param option
 * @return
 */
int check_if_correct(Option *option) {
    if (option->records < 1) {
        printf("\nAmount of records can not be less than 1!\n");
        return -1;
    }
    if (option->recordSize < 1) {
        printf("\nRecord size can not be less than 1!\n");
        return -2;
    }
    if (!option->option == 'l' && !option->option == 's') {
        printf("\nVersion of program are:\ns-for system functions\nl - for library functions");
        return -2;
    }

    return 0;


}

/**
 * descript function given in arguments
 * @param function given in string
 * @return
 */
enum Functions descript_function(char *function) {
    if (strcmp(function, "generate") == 0) return generate;
    if (strcmp(function, "shuffle") == 0) return shuffle;
    if (strcmp(function, "sort") == 0) return sort;
    printf("\nUnknown function!\n");
//    return NULL;


}

/**
 * return container with arguments
 * @param count of args
 * @param argv - array of strings
 * @return
 */
Option *parseOption(int a, char *argv[]) {
    Option *option = malloc(sizeof(option));
    enum Arguments arg;
    int i;
    for (i = 1; i < a; i++) {//-file -u -fsize -rsize -
        arg = descript(argv[i]);
        switch (arg) {
            case file:
                option->fileName = argv[i + 1];
                break;
            case version:
                option->option = argv[i + 1][0];
                break;
            case file_size:
                option->records = atoi(argv[i + 1]);
                break;
            case record_size:
                option->recordSize = atoi(argv[i + 1]);
                break;
            case function:
                option->function = descript_function(argv[i + 1]);
                break;

            default:
                break;

        }
    }
    return option;

}

void display_man() {
    printf("\nUse:");
    //-file, -version, -fsize, -rsize, -fun
    printf("\n\t-file\t<file name>\t\tfor specify a file to create/open");
    printf("\n\t-fun\tgenerate|sort|shuffle\tfor specify a function to execute");
    printf("\n\t-fsize\t<amount of records>");
    printf("\n\t-rsize\t<amount of bytes per record>");
    printf("\n\t-version\tl|s\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        srand(time(NULL));
        struct Option *options = parseOption(argc, argv);
        if (check_if_correct(options) == 0)
            switch (options->function) {
                case generate:
                    generate_records(options->fileName, options->recordSize, options->records);
                    break;
                case shuffle:
                    shufflee(options->fileName, options->recordSize, options->records, options->option);
                    break;
                case sort:
                    bubble_sort(options->fileName, options->records, options->recordSize, options->option);

                    break;
                default:
                    printf("\nUnknown function\nUse -fun {generate, shuffle, sort}\n");
            }
    } else display_man();


}
