//
// Created by przemek on 10.05.17.
//

#include <fcntl.h>
#include <pthread.h>
#include <zconf.h>
#include <string.h>
#include "finderv1.h"

int threads_amount;
char *file_name;
int records_to_read;
char *to_find;
int records_size = 1024;
int fd;
int END_FILE = 0;
int *finished;


int main(int argc, char *argv[]) {

    if (argc < 5) {
        printf("Usage: finder threads_amount file_name records_to_read to_find\n");
        exit(0);
    } else {
        threads_amount = atoi(argv[1]);
        file_name = argv[2];
        records_to_read = atoi(argv[3]);
        to_find = argv[4];
    }

    fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("File doesn't exists!");
        exit(-1);
    }

    int err;
    finished = malloc(threads_amount * sizeof(int));
    for (int j = 0; j < threads_amount; j++)
        finished[j] = 1;

    pthread_t *threads = malloc(threads_amount * sizeof(pthread_t));
    int iterator = 0;
    while (!END_FILE) {
        if (finished[iterator]) {
            finished[iterator] = 0;
            err = pthread_create(&threads[iterator], NULL, &do_some_thing, iterator);
            if (err == -1) {
                perror("erroor while creating thread\n");
            }


        }
        iterator = (iterator + 1) % threads_amount;


    }


    for (int t = 0; t < threads_amount; t++) {
        err = pthread_create(&threads[t], NULL, &do_some_thing, NULL);
        if (err == -1) {
            perror("erroor while creating thread\n");
        }

    }






/*

    pthread_t *threads = malloc(threads_amount*sizeof(pthread_t));
    int err = 0;
    for (int t = 0 ; t<threads_amount; t++){
        err = pthread_create(&threads[t], NULL, &do_some_thing, NULL );
        if (err == -1){
            perror("Error while creating thread\n");
        }else{
            printf("Created succsefulyl\n");
        }
        pthread_join(&threads[t], NULL);
    }
*/
    return 0;
}

void *do_some_thing(void *arg) {
    //Read_to_local_
    int i = (int) arg;


    printf("start read\n");
    void *buff = malloc(sizeof(char) * 1024 * records_to_read);
    ssize_t bytes_read;

    bytes_read = read(fd, buff, sizeof(char) * 1024 * records_to_read);
    if (bytes_read == 0) {

        END_FILE = 1;
    } else {
        printf("\nreaded %d bytes\n", records_size * records_to_read);
    }
    int index = 1;
    char *record = malloc(sizeof(char) * 1020);

    for (int r = 0; r < records_to_read; r++) {
        memcpy(&index, buff, sizeof(int));
        buff += sizeof(int);
        memcpy(record, buff, 1020);
        printf("%d. %s \n", index, record);
        buff += 1020;

    }
    finished[i] = 1;


}