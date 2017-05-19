//
// Created by przemek on 10.05.17.
//
#include <fcntl.h>
#include <zconf.h>
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char *argv[]) {
    if (argc == 3)
        printf("Creating %s file with %s records", argv[1], argv[2]);
    else {
        printf("\n\nusage file_creator filename records\n\n");
        exit(-1);
    }
    char *fname = argv[1];
    int records = atoi(argv[2]);

    int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("Error while open()");
        exit(-1);
    }
    char byte;
    for (int i = 0; i < records; i++) {
        printf("\n%lf %%", i * 100.0 / records);
        if (write(fd, &i, sizeof(int)) == -1) {
            perror("Write error");
            exit(-1);
        }
        for (int e = 0; e < 1024 - sizeof(int); e++) {
            byte = (char) ('a' + rand() % 26);
            write(fd, &byte, sizeof(byte));

        }
    }
    close(fd);

    return 0;


}