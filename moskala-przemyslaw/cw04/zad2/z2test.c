#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <memory.h>

//
// Created by przemek on 30.03.17.
//
pid_t PPID;

void simulate_work() {
    printf("\n %d working\n", getpid());
    sleep(1);
}
void recieve(int sig, siginfo_t *siginfo, void *context) {
    printf("%d recieved sigusrf from %d", getpid(), siginfo->si_pid);

}

void send_request() {
    printf("PID  %d sending request to PPID: %d\n", getpid(), getppid());
    //kill(getppid(),SIGUSR1);  ?logout
    kill(PPID,SIGUSR1);
}

void create_children(int n) {
    struct sigaction act;
    memset(&act,'\0',sizeof(act));
    act.sa_sigaction=&recieve;
    act.sa_flags=SA_SIGINFO;
    printf("[dad] pid %d\n", getpid());

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            simulate_work();
            send_request();


            exit(0);
        } else {
            if (pid>0) {

                sigaction(SIGUSR1,&act, NULL);
                wait(NULL);
            }

        }
    }


}


int main(int argc, char *argv[]) {
    PPID=getpid();
    create_children(atoi(argv[1]));
}