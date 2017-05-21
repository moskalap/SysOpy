//
// Created by przemek on 21.05.17.
//

#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <zconf.h>
#include "reader_writer_posix.h"

int read_count = 0;
int READERS = 0;
int WRITERS = 0;
int verbose = 0;
int N = 0;
int *T;
int sleep_time;
sem_t *res_sem;
sem_t *r_sem;

int run = 1;
int mod = 21;
void end_fun(int a) {
    run = 0;
}

void sem_post_er(sem_t *a) {
    if (-1 == sem_post(a)) {
        perror("sempost");
        kill(getpid(), SIGINT);
    }
}

void sem_wait_er(sem_t *a) {
    if (-1 == sem_wait(a)) {
        perror("semwait");
        kill(getpid(), SIGINT);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    signal(SIGINT, end_fun);
    parse_args(argc, argv);
    destroy_semaphore(res_sem, "rw_sem");
    destroy_semaphore(r_sem, "r_sem");
    res_sem = make_semaphore("rw_sem");
    r_sem = make_semaphore("r_sem");
    T = malloc(N * sizeof(int));
    pthread_t readers[READERS];
    pthread_t writers[WRITERS];

    for (int i = 0; i < WRITERS; i++) {
        pthread_create(&writers[i], NULL, &do_writer_job, NULL);
    }

    for (int i = 0; i < READERS; i++) {
        pthread_create(&readers[i], NULL, &do_reader_job, NULL);
    }


    for (int i = 0; i < READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }


    destroy_semaphore(res_sem, "rw_sem");
    destroy_semaphore(r_sem, "r_sem");
}


void do_writer_job(void *arg) {
    int mods;
    int pos;
    int val;
    while (run) {
        sem_wait_er(res_sem);
        mods = rand() % N;

        for (int i = 0; i < mods; i++) {
            pos = rand() % N;
            val = rand() % 100000;
            T[pos] = val;
            if (verbose)
                printf("writer %ld:\t T[%d]=%d\n", pthread_self(), pos, val);
        }

        sem_post_er(res_sem);
    }

}

void do_reader_job(void *arg) {


    int found = 0;
    while (run) {
        sem_wait_er(r_sem);
        read_count++;
        if (read_count == 1)
            sem_wait_er(res_sem);
        sem_post_er(r_sem);


        found = 0;
        for (int i = 0; i < N; i++) {

            if (T[i] % mod == 0) {
                found++;
                if (verbose)
                    printf("reader %ld:\tT[%d] = %d\n", pthread_self(), i, T[i]);
            }
        }
        printf("reader %ld:\tFound %d numbers\n", pthread_self(), found);

        sem_wait(r_sem);
        read_count--;
        if (read_count == 0)
            sem_post_er(res_sem);
        sem_post_er(r_sem);
        usleep(sleep_time);
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

sem_t *make_semaphore(char *a) {

    sem_t *sem = sem_open(a, O_CREAT | O_EXCL | O_RDWR, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("init sem error");
        exit(-1);
    }

    return sem;
}

void destroy_semaphore(sem_t *a, char *b) {
    if (0 != sem_close(a)) {
        perror("destroy sem error");
        //exit(-1);
    }
    if (0 != sem_unlink(b)) {
        perror("destroy sem error");
        exit(-1);
    }
}

