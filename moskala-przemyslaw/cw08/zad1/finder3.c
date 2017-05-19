//
// Created by przemek on 15.05.17.
//

//
// Created by przemek on 10.05.17.
//

#include <fcntl.h>
#include <pthread.h>
#include <zconf.h>
#include <string.h>
#include "finder1.h"

int threads_amount;
char *file_name;
int records_to_read;
char *to_find;
int records_size = 1024;
int fd;
int END_FILE = 0;
int records_transfered;
int *finished;
pthread_key_t records;
pthread_t *threads;
pthread_mutex_t out_mutex = PTHREAD_MUTEX_INITIALIZER;


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
    //ids = malloc(sizeof(pthread_key_t)*records_to_read);
    //records = malloc(sizeof(pthread_key_t)*records_to_read);
    pthread_key_create(&records, NULL);


    threads = malloc(threads_amount * sizeof(pthread_t));


    for (int t = 0; t < threads_amount; t++) {
        err = pthread_create(&threads[t], NULL, &do_some_thing, NULL);
        if (err == -1) {
            perror("erroor while creating thread\n");
        }

    }

    for (int t = 0; t < threads_amount; t++)
        pthread_join(threads[t], NULL);

    for (int r = 0; r < records_to_read; r++) {
        pthread_key_delete(records);

    }

    return 0;
}

void *do_some_thing(void *arg) {

    pthread_t myTID = pthread_self();
    while (!END_FILE) {
        if (read_file()) {

            END_FILE = 1;
        } else {

            void *buff;

            buff = pthread_getspecific(records);

            int index;
            char *record = malloc(sizeof(char) * 1020);


            for (int r = 0; r < records_to_read; r++) {
                memcpy(&index, buff, sizeof(int));
                buff += sizeof(int);
                memcpy(record, buff, 1020);
                if (record)
                    if (strstr(record, to_find) != NULL) {
                        pthread_mutex_lock(&out_mutex);
                        printf("TID: %ld Found %s in\n %d %s\n", myTID, to_find, index, record);
                        pthread_mutex_unlock(&out_mutex);
                    }
                buff += 1020;

            }
        }
    }


}

int read_file() {
    void *buff = malloc(sizeof(char) * 1024 * records_to_read);
    ssize_t bytes_read;
    bytes_read = read(fd, buff, sizeof(char) * 1024 * records_to_read);
    pthread_setspecific(records, buff);
    return !bytes_read;
}