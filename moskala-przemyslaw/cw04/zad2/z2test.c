#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

//
// Created by przemek on 30.03.17.
//
void simulate_work() {
    printf("\nworking\n");
    sleep(1);
}

void send_request() {
    printf("PID: %d PPID: %d", getpid(), getppid());
}

void create_children(int n) {
    printf("[dad] pid %d\n", getpid());

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("[son] pid %d from pid %d\n", getpid(), getppid());
            exit(0);
        } else {
            if (pid > 0) {
                printf("IM parent %d\n", getpid());
                wait(NULL);
            }

        }
    }


}


int main(int argc, char *argv[]) { //N M
    create_children(atoi(argv[1]));
}