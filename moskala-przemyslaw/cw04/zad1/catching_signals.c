#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int delta = 1;
int i;

void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("Recievied SIGINT\n");
        _exit(0);
    } else if (signo == SIGTSTP) {
        delta = delta * (-1);
    }


}

int main(void) {

    if (signal(SIGTSTP, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGTstOP\n");
    signal(SIGINT, sig_handler);
    // A long long wait so that we can easily issue a signal to this process
    while (1) {
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