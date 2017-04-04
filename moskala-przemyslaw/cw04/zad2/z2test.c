#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <memory.h>
#include <time.h>
#include <bits/siginfo.h>

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
    sleep(1);
}

void recieve_sigrt(int sig, siginfo_t *siginfo, void* context){
    printf(">Recieved SIGRT%d from PID%d\n", siginfo->si_signo, siginfo->si_pid);
}
void recieve_request(int sig, siginfo_t *siginfo, void *context) {
    printf(">Recieved SIGUSR1 (request) from %d\n", getpid(), siginfo->si_pid);
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
        //printf("%d recieved permission\n",getpid());
    }
}
void send_random_signal(pid_t pid){
    srand(time(NULL));
    int i = rand()%(SIGRTMAX-SIGRTMIN) + SIGRTMIN;
    //printf("wysyłam %d do rod\n",i);
    kill(pid, i);
}

void send_request() {
    printf("PID  %d sending request to PPID: %d\n", getpid(), getppid());
    //kill(getppid(),SIGUSR1);  ?logout
    kill(PPID,SIGUSR1);
}

void create_children(int n) {
    struct sigaction act;
    pid_t pids[n];
    int status[n];
    struct sigaction rtact;
    memset(&act,'\0',sizeof(act));
    memset(&rtact,'\0',sizeof(act));
    act.sa_sigaction=&recieve_request;
    rtact.sa_sigaction=&recieve_sigrt;
    act.sa_flags=rtact.sa_flags=SA_SIGINFO;
    printf("[dad] pid %d\n", getpid());
    int i;

    for (i = SIGRTMIN; i < SIGRTMAX; i++)
        sigaction(i, &rtact, NULL);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            simulate_work();
            send_request();
            time_t start=clock();
            pause();
            printf("%d konccze sie\n", getpid());
            send_random_signal(PPID);
            time_t end=clock();
            int a=(int) difftime(end,start);
            printf("%d konccze sie z czasem %d \n", getpid(),a);

            exit((int) difftime(end,start));
        } else {
            if (pid>0) {
                pids[i]=pid;

                sigaction(SIGUSR1,&act, NULL);
                waitpid(pids[i],&status[i],0);
                if(WIFEXITED(status[i])){
                    printf("%d exited with%d\n", pids[i], WIFEXITED(status[i]));
                }
            }

        }
    }


}


int main(int argc, char *argv[]) {
    srand(time (NULL));
    K=atoi(argv[2]);
    waiters=malloc(sizeof(pid_t)*K);
    signal(SIGUSR2, get_permission);
    PPID=getpid();



    create_children(atoi(argv[1]));
    while (1);
}