//
// Created by przemek on 18.04.17.
//

#include <stdlib.h>
#include "stdio.h"
#include <string.h>
#include <zconf.h>
#include <printf.h>
#include "client.h"
#include "common.h"
#include "server.h"

int run=1;
int private_queue;
int public_queue;
pid_t myID;

pid_t server=0;

long get_type(char msg[]) {
    if(msg[0] == ':'){
        switch (msg[1]){
            case 'e':
                return ECHO;
            case 't':
                return TIME_REQ;
            case 'q':
                run=0;
                return TERM;
            case 'u':
                return TO_UPPER;
            default:
                return -1;
        }
    }
    return -1;

}

char *str_cut(char *str, int begin, int len) {
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return str;
}

key_t get_key() {
    key_t key = ftok(getenv(PATH), PROJ_ID);
    if (key == -1) {
        fprintf(stderr, "error while creating key");
        exit(key);
    }
    return key;

}

int get_queue(key_t key, int flags) {
    int q = msgget(key, flags);
    if (q == -1) {
        fprintf(stderr, "error while %s queue", key == IPC_PRIVATE ? "creating private" : "opening public");
    }
}
void display_message(Message* msg){
    printf("From: %d\nType: %s\nVal: %s\n", msg->sender,
           (msg->type == ECHO) ? "echo" :
           ((msg->type == TO_UPPER) ? "to_upper" :
            (msg->type == TIME_REQ) ? "time" :
            (msg->type == TERM) ? "term" :
            "noth"),
           msg->value);

}

void send_message(Message msg, int queue) {
    if (msgsnd(queue, &msg, MESSAGE_SIZE, 0) == -1) {
        fprintf(stderr, "error while sending a message %s\n", msg.value);
        msgctl(private_queue, IPC_RMID, NULL);
        exit(-1);
    }

}


int login() {
    char private_q_str[25];

    Message msg;
    sprintf(msg.value, "%d", private_queue);
    msg.sender = getpid();
    msg.type = LOGIN;
    send_message(msg, public_queue);

    sleep(1);
    msgrcv(private_queue, &msg, (size_t) MESSAGE_SIZE, 0, MSG_NOERROR);
    if (strcmp(msg.value, ":(") == 0) {
        printf("failure");
    } else {
        printf("%s", msg.value);
        myID = atoi(msg.value);
    }



}
int main(){

    key_t sever_key = get_key();
    private_queue = get_queue(IPC_PRIVATE, 0666);
    public_queue = get_queue(sever_key, 0);
    int id = login();
    printf("Connection successful\n"
                   "use \n"
                   ":e TEXT\t\tfor echo\n"
                   ":u TEXT\t\tfor translate to upper case\n"
                   ":t\t\t\tto receive time\n"
                   ":q\t\t\tfor terminate server \n"
    );

    char *buffer;
    size_t bufsize = 32;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));
    Message msg;
    char * b;
    long type;
    while(run){

        printf("\n%d@serv: >\t ", myID);
        getline(&buffer,&bufsize,stdin);
        type=get_type(buffer);
        b = str_cut(buffer,0,2);
        strcpy(msg.value, b);
        msg.sender = myID;
        msg.type = type;
        send_message(msg, public_queue);
        if (type == TERM) {
            msgctl(private_queue, IPC_RMID, NULL);
            exit(0);
        }
        msgrcv(private_queue, &msg, (size_t) MESSAGE_SIZE, 0, MSG_NOERROR);
        printf("%s", msg.value);




    }


}