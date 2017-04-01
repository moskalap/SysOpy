#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <memory.h>

//
// Created by przemek on 30.03.17.
//
pid_t PPID;
int permision=0;
int K;
int RECIEVED=0;
pid_t * waiters;
void simulate_work(){
    printf("\n %d working\n", getpid());
    sleep(10);
}
void recieve_request(int sig, siginfo_t *siginfo, void *context) {
    printf("%d recieved REQUEST from %d\n", getpid(), siginfo->si_pid);
    waiters[RECIEVED]=siginfo->si_pid;
    RECIEVED++;
    if(RECIEVED==K){
        for( int i = 0; i<K; i++)
            send_permission(waiters[i]);
    }
    if(RECIEVED>K) send_permission(siginfo->si_pid);


}
void send_permission(pid_t pid){ //SIGUSR2 -perm
kill(pid, SIGUSR2);
}
void get_permission(int signo){
    if (signo == SIGUSR2) {
        printf("%d recieved permission",getpid());
    }
}

void send_request() {
    printf("PID  %d sending request to PPID: %d\n", getpid(), getppid());
    //kill(getppid(),SIGUSR1);  ?logout
    kill(PPID,SIGUSR1);
}

void create_children(int n) {
    struct sigaction act;

    memset(&act,'\0',sizeof(act));
    act.sa_sigaction=&recieve_request;
    act.sa_flags=SA_SIGINFO;
    printf("[dad] pid %d\n", getpid());

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            simulate_work();
            send_request();
            pause();



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
    K=atoi(argv[2]);
    waiters=malloc(sizeof(pid_t)*K);
    signal(SIGUSR2, get_permission);
    PPID=getpid();
    create_children(atoi(argv[1]));
}