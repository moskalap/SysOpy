//
// Created by przemek on 21.05.17.
//


#include <zconf.h>
#include <pthread.h>
#include <stdio.h>
#include "reader_writer_mutex.h"

int read_count = 0;
int write_count = 0;
int READERS = 0;
int WRITERS = 0;
int verbose = 0;
int N = 0;
int *T;
int sleep_time;
pthread_mutex_t res_sem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t r_sem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t w_sem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t try_read_sem = PTHREAD_MUTEX_INITIALIZER;


int run = 1;
int mod = 21;

void end_fun(int a) {
    run = 0;
}

void sem_post_er(pthread_mutex_t *a) {
    if (-1 == pthread_mutex_unlock(a)) {
        perror("sempost");
        kill(getpid(), SIGINT);
    }
}

void sem_wait_er(pthread_mutex_t *a) {
    if (-1 == pthread_mutex_lock(a)) {
        perror("semwait");
        kill(getpid(), SIGINT);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    signal(SIGINT, end_fun);
    parse_args(argc, argv);

    make_semaphore();

    T = malloc(N * sizeof(int));
    pthread_t readers[READERS];
    pthread_t writers[WRITERS];

    for (int i = 0; i < READERS; i++) {
        pthread_create(&readers[i], NULL, &do_reader_job, NULL);
    }

    for (int i = 0; i < WRITERS; i++) {
        pthread_create(&writers[i], NULL, &do_writer_job, NULL);
    }


    for (int i = 0; i < READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

}

void do_writer_job(void *arg) {
    int mods;
    int pos;
    int val;
    while (run) {

        sem_wait_er(&w_sem);
        write_count++;
        if (write_count == 1)
            sem_wait_er(&try_read_sem);
        sem_post_er(&w_sem);

        sem_wait_er(&res_sem);
        mods = rand() % N;

        for (int i = 0; i < mods; i++) {
            pos = rand() % N;
            val = rand() % 100000;
            T[pos] = val;
            if (verbose)
                printf("writer %ld:\t T[%d]=%d\n", pthread_self(), pos, val);
        }

        sem_post_er(&res_sem);


        sem_wait_er(&w_sem);
        write_count--;
        if (write_count == 0)
            sem_post_er(&try_read_sem);
        sem_post_er(&w_sem);
        usleep((__useconds_t) sleep_time);
    }


}

void do_reader_job(void *arg) {

    int found = 0;
    while (run) {
        sem_wait_er(&try_read_sem);
        sem_wait_er(&r_sem);
        read_count++;


        if (read_count == 1)
            sem_wait_er(&res_sem);
        sem_post_er(&r_sem);
        sem_post_er(&try_read_sem);


//critsection

        found = 0;
        for (int i = 0; i < N; i++) {

            if (T[i] % mod == 0) {
                found++;
                if (verbose)
                    printf("reader %ld:\tT[%d] = %d\n", pthread_self(), i, T[i]);
            }
        }
        printf("reader %ld:\tFound %d numbers\n", pthread_self(), found);


//!critsection


        sem_wait_er(&r_sem);
        read_count--;
        if (read_count == 0)
            sem_post_er(&res_sem);
        sem_post_er(&r_sem);
        usleep((__useconds_t) sleep_time);
    }

}

void parse_args(int argc, char *argv[]) {
    if (argc > 5) {
        READERS = atoi(argv[1]);
        WRITERS = atoi(argv[2]);
        N = atoi(argv[3]);
        mod = atoi(argv[4]);
        sleep_time = atoi(argv[5]);
        if (argc == 7) verbose = 1;
    } else {
        printf("usage: ./reader_writer_posix READERS WRITERS N DIVISOR SLEEPTIME");
        exit(0);
    }
}

void make_semaphore() {


    if (-1 == pthread_mutex_init(&r_sem, NULL)) {
        perror("init sem error");
        exit(-1);
    }


    if (-1 == pthread_mutex_init(&w_sem, NULL)) {
        perror("init sem error");
        exit(-1);
    }


    if (-1 == pthread_mutex_init(&res_sem, NULL)) {
        perror("init sem error");
        exit(-1);
    }


    if (-1 == pthread_mutex_init(&try_read_sem, NULL)) {
        perror("init sem error");
        exit(-1);
    }


}



