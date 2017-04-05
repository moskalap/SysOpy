#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <memory.h>
#include <time.h>

//
// Created by przemek on 30.03.17.
//
int run=1;
int N;
int K;
int ended=0;
pid_t PPID;
int permision=0;
int k_reached=0;
int recieved=-1;
pid_t * pids;
sigset_t new;
sigset_t old;

void simulate_work(){
    printf("\n %d working\n", getpid());
    sleep(10);
}
void accept(pid_t pid){
    printf("%d Sent permission(SIGUSR2) to %d \n", PPID,pid);
    kill(pid, SIGUSR2);

}


void parent_handler(int sig, siginfo_t *siginfo, void* context){

    int sign=siginfo->si_signo;
    switch (sign){
        case SIGUSR1: //request
            //printf("Got SIGUSR\n");
            recieved++;
            if(k_reached){
                accept(siginfo->si_pid);
            }else{
                if(recieved==K) {
                    k_reached=1;
                    int j=0;
                    for( j ; j<K; j++)
                        accept(pids[j]);
                    accept(siginfo->si_pid);
                }
                else{
                    pids[recieved]=siginfo->si_pid;
                    printf("Add to wait %d \n",siginfo->si_pid);
                }

            }


            break;
        case SIGINT:
            printf("Recieved SIGINT");
            for (int i =0 ; i<N; i++){
                kill(pids[i], SIGINT);
            }

            break;
        case SIGCHLD:
            ended++;
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%d exited with status %d<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n", siginfo->si_pid, siginfo->si_status);
            if(ended==N) run=0;
            break;

        default:
            if(sign>SIGRTMIN && sign<SIGRTMAX)
                printf("Recieved SIGRT%d from %d.\n", sig-SIGRTMIN, siginfo->si_pid);
            break;


    }
}
void child_handler(int sig, siginfo_t *siginfo, void* context){
    int sign = siginfo->si_signo;
    switch(sign){
        case SIGUSR2://RECIEVED PERM
            printf("%d recieved permision.\n", getpid());

            break;
        default:
            break;

    }
}






void send_request(){
    printf("\t%d sent request(SIGUSR1) to %d\n",getpid(),PPID);


    kill(PPID,SIGUSR1);
}
void send_random(){
    // srand(time(NULL));
    int i = rand()%(SIGRTMAX-SIGRTMIN) + SIGRTMIN;
    //printf("wysyłam %d do rod\n",i);
    kill(PPID, i);
}


void usrhndl (int signo) {
    printf("recievied");
    if (signo == SIGUSR1) {
        printf("AAA");
        //
        // delta = delta * (-1);
    }


}


void do_child_things(){


    //PPID=getppid();
    srand(getpid());
    //PPID=getppid();

    printf("im %d of %d\nStarting work..(set ppid %d)\n",getpid(),getppid(),PPID);

    int r=rand()%10+1;
    sleep(r);
    //send_request();

    time_t start;
    time(&start);
    printf("%d konccze sie %d \n", getpid(),r);
    //send_random_signal(PPID);
    sleep(2);
    time_t end;
    time(&end);
    int a=(int) difftime(end,start);
    exit((int) difftime(end,start));




}




void create_children(int n) {
    PPID=getpid();
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            srand(getpid());
            int r = rand()%10 +1;

            printf("\tChild%d (of %d) will be working fro %d sec..\n", getpid(),PPID,r);
            sleep(r);
            send_request();
            struct timespec tstart={0,0}, tend={0,0};
            clock_gettime(CLOCK_MONOTONIC, &tstart);

            pause();

            send_random();

            clock_gettime(CLOCK_MONOTONIC, &tend);
            double dtime = (((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec))*10000;
            //printf("some_long_computation took about %.5f miliseconds\n", dtime );
            exit((int)dtime);

        } else {
            if (pid>0) {

                pids[i]=pid;
                //printf("parent of %d \n", pid);





            }

        }
    }
    //sleep(20);
    wait(NULL);

    printf("end");


}


int main(int argc, char *argv[]) {

    struct sigaction parent;
    struct sigaction children;
    memset(&parent,'\0',sizeof(parent));
    memset(&children,'\0',sizeof(children));
    parent.sa_sigaction=&parent_handler;
    children.sa_sigaction=&child_handler;
    parent.sa_flags=children.sa_flags=SA_SIGINFO;
    sigemptyset(&parent.sa_mask);
    sigaddset(&parent.sa_mask, SIGUSR1);
    int s = SIGRTMIN;
    for(s; s<SIGRTMAX; s++)
        sigaddset(&parent.sa_mask, s );
    sigaddset(&parent.sa_mask, SIGCHLD );

    sigaction(SIGCHLD,&parent,NULL);
    sigaction(SIGUSR1,&parent,NULL);
    sigaction(SIGUSR2,&children,NULL);
    s = SIGRTMIN;
    for(s; s<SIGRTMAX; s++)
        sigaction(s,&parent,NULL);
    //signal(SIGUSR1, usrhndl);

    N=atoi(argv[1]);
    pids=malloc(N*sizeof(pid_t));
    K=atoi(argv[2]);
    PPID=getpid();
    printf("PPID %d\n",PPID);
    create_children(atoi(argv[1]));

    while(run);
}