#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include <memory.h>
#include <stdlib.h>

int delta = 1;
int i;
int run = 1;

void sig_handler(int signo) {

    if (signo == SIGTSTP) {
        delta = delta * (-1);
    }
}

void hdl(int sig, siginfo_t *siginfo, void *context) {
    printf("recieved sigint");
    exit(0);
}

int main(void) {

    if (signal(SIGTSTP, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGTstOP\n");
    signal(SIGINT, sig_handler);
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_sigaction = &hdl;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);
    


    while (run) {
        for (i; i < 26 && i >= 0; i += delta) {
            printf("%c \n", 'a' + i);
            sleep(1);
        }
        if (delta == 1)
            i = 0;
        else i = 25;
    }
    return 0;
}