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
int verbose = 1;
int N = 0;
int *T;
sem_t *res_sem;
sem_t *r_sem;
sem_t *w_sem;
int run = 1;

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
    res_sem = make_semaphore("rw_sem");
    //w_sem=make_semaphore("w_sem");
    r_sem = make_semaphore("r_sem");
    T = malloc(N * sizeof(int));
    pthread_t reader;
    pthread_t writer;
    int a = 1;
    pthread_create(&reader, NULL, &do_reader_job, &a);
    pthread_create(&writer, NULL, &do_writer_job, NULL);


    pthread_join(reader, NULL);
    pthread_join(writer, NULL);


    destroy_semaphore(res_sem, "rw_sem");
    destroy_semaphore(r_sem, "r_sem");
    // destroy_semaphore(w_sem, "w_sem");
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
            val = rand() % N;
            T[pos] = val;
            if (verbose)
                printf("writer:\t T[%d]=%d\n", pos, val);
        }

        sem_post_er(res_sem);
    }

}

void do_reader_job(void *arg) {

    int mod = (int) arg;
    int found = 0;
    while (run) {
        sem_wait(r_sem);
        read_count++;
        if (read_count == 1)
            sem_wait(res_sem);
        sem_post_er(r_sem);


        found = 0;
        for (int i = 0; i < N; i++) {

            if (T[i] % mod == 0) {
                found++;
                if (verbose)
                    printf("reader\tT[%d] = %d\n", i, T[i]);
            }
        }
        printf("reader\tFound %d numbers", found);

        sem_wait(r_sem);
        read_count--;
        if (read_count == 0)
            sem_post_er(res_sem);
        sem_post_er(r_sem);
    }

}

void parse_args(int argc, char *argv[]) {
    if (argc == 4) {
        READERS = atoi(argv[1]);
        WRITERS = atoi(argv[2]);
        N = atoi(argv[3]);
    } else {
        printf("usage: ./reader_writer_posix READERS WRITERS N");
        exit(0);
    }
}

sem_t *make_semaphore(char *a) {

    sem_t *sem = sem_open(a, O_CREAT | O_EXCL | O_RDWR, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("init sem error");
        // exit(-1);
    }

    return sem;
}

void destroy_semaphore(sem_t *a, char *b) {
    if (0 != sem_close(a)) {
        perror("destroy sem error");
        // exit(-1);
    }
    if (0 != sem_unlink(b)) {
        perror("destroy sem error");
        exit(-1);
    }
}

