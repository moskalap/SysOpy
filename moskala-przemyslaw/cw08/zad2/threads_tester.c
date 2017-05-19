//
// Created by przemek on 10.05.17.
//
#include <zconf.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "threads_tester.h"

pthread_t *threads;

void sighandler(int s) {
    switch (s) {
        case SIGUSR1:
            printf("PID:%d\tTID:%ld\t dostałem SIGUSR1", getpid(), pthread_self());
            break;
        case SIGTERM:
            printf("PID:%d\tTID:%ld\t dostałem SIGTERM", getpid(), pthread_self());
            break;
        case SIGKILL:
            printf("PID:%d\tTID:%ld\t dostałem SIGKILL", getpid(), pthread_self());
            break;
        case SIGSTOP:
            printf("PID:%d\tTID:%ld\t dostałem SIGSTOP", getpid(), pthread_self());
            break;


    }
}

int main() {
    /** 2)
     *
     sigset_t sigset;
     sigfillset(&sigset);
     sigprocmask(SIG_SETMASK,&sigset, NULL);
     */


    int THREADS_AMNT = 4;
    printf("Tworzę %d wątków.\n", THREADS_AMNT);
    threads = malloc(THREADS_AMNT * sizeof(pthread_t));
    for (int i = 0; i < THREADS_AMNT - 1; i++) {
        pthread_create(&threads[i], NULL, &do_some_normal_things, NULL);
    }
    pthread_create(&threads[3], NULL, &do_some_crazy_things, NULL);

    sleep(10);


}

void *do_some_normal_things(void *arg) {
    signal(SIGUSR1, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGSTOP, sighandler);
    signal(SIGKILL, sighandler);
    while (1) {

        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        //printf ( "Current local time and date: %s", asctime (timeinfo) );
        printf("%s\t TID:%ld \t Działam.\n\n", asctime(timeinfo), pthread_self());
        sleep(1);
    }

}

void *do_some_crazy_things(void *arg) {
    while (1) {
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        //printf ( "Current local time and date: %s", asctime (timeinfo) );
        printf("%s\t TID:%ld \t Działam i zaraz podziele przez 0.\n\n", asctime(timeinfo), pthread_self());
        sleep(1);
        int i = 4 / 0;
    }
}