//
// Created by przemek on 04.04.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <signal.h>

int recieved_from_child=0;
int recieved_from_parent=0;
int sent_to_parent=0;
void send_using_sigqueue(int L){
    int a=1;
}
void send_rt_signals(int L){
    int b=1;


}
void send_using_kill(int L){
    pid_t pid=fork();

    if (pid==0){
        printf("dziecko mowi %d\n", getpid());

    }
    else if (pid>0){
        printf("rodzi utworzyl %d\n", pid);
        for(int i=0 ; i<L; i++)
            kill(pid,SIGUSR1);


    }

}

int main(int argc, char * argv[]){
    int L=atoi(argv[1]);
    int type=atoi(argv[2]);
    switch(type){
        case 1:
            send_using_kill(L);
            break;
        case 2:
            send_using_sigqueue(L);
            break;
        case 3:
            send_rt_signals(L);
            break;
        default:
            printf("Type don't recognized!");
            break;

    }

    printf("ada");
    return 0;
}