//
// Created by przemek on 18.04.17.
//

#include <stdlib.h>
#include "stdio.h"
#include <string.h>
#include <zconf.h>
#include <printf.h>

#include <mqueue.h>
#include <sys/time.h>
#include <time.h>
#include "client.h"
#include "common.h"
#include "server.h"

int run = 1;
mqd_t private_queue;
mqd_t public_queue;
pid_t myID;


pid_t server = 0;

long get_type(char msg[]) {
    if (msg[0] == ':') {
        switch (msg[1]) {
            case 'e':
                return ECHO;
            case 't':
                return TIME_REQ;
            case 'q':
                run = 0;
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

void display_message(Message *msg) {
    printf("From: %d\nType: %s\nVal: %s\n", msg->sender,
           (msg->type == ECHO) ? "echo" :
           ((msg->type == TO_UPPER) ? "to_upper" :
            (msg->type == TIME_REQ) ? "time" :
            (msg->type == TERM) ? "term" :
            "noth"),
           msg->value);

}

void send_message(Message *msg, mqd_t queue) {

    int a = MESSAGE_SIZE;
    char *v = (char *) msg;
    if (mq_send(queue, v, a, 0) == -1) {
        fprintf(stderr, "error while sending a message %s\n", msg->value);
        msgctl(private_queue, IPC_RMID, NULL);
        exit(-1);
    }

}

generate_name(char *s) {
    const char charset[] = "abcdefghijklmnopqrstuvwxy";
    int i;
    for (i = 0; i < MAX_NAME_L; i++) {
        int key = rand() % (int) (sizeof charset - 1);
        s[i] = charset[key];
    }
    s[i] = '\0';
    s[0] = '/';

}

mqd_t get_public_queue() {
    mqd_t q = mq_open(SERVER_NAME, O_WRONLY);
    if (q == -1) {
        printf("error while opening a public queue\n");
        exit(-1);
    }
    return q;

}

mqd_t create_private_queue(char *name) {
    mqd_t q;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MSGS_MAX;
    attr.mq_msgsize = MSG_SIZE;
    q = mq_open(name, O_CREAT | O_RDONLY, 0666, &attr);
    if (q == -1) {
        fprintf(stderr, "error while creating a private queue\n");
        exit(-1);
    }
    return q;
}

int login() {
    char private_q_str[25];

    Message msg;
    sprintf(msg.value, "%d", private_queue);
    msg.sender = getpid();
    msg.type = LOGIN;
    send_message(&msg, public_queue);

    sleep(1);
    msgrcv(private_queue, &msg, (size_t) MESSAGE_SIZE, 0, MSG_NOERROR);
    if (strcmp(msg.value, ":(") == 0) {
        printf("failure");
    } else {
        printf("%s", msg.value);
        myID = atoi(msg.value);
    }


}

int main() {
    srand(time(NULL));


    char private_queue_name[MAX_NAME_L];
    generate_name(private_queue_name);
    printf("Private queue: %s\n", private_queue_name);

    public_queue = get_public_queue();
    private_queue = create_private_queue(private_queue_name);
    int id = login();


    /*
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

         printf("\n%d@serv: >\t ",myID);
         getline(&buffer,&bufsize,stdin);
         type=get_type(buffer);
         b = str_cut(buffer,0,2);
         strcpy(msg.value,b);
         msg.sender = myID;
         msg.type=type;
         send_message(msg,public_queue);
         if(type == TERM){
             msgctl(private_queue, IPC_RMID, NULL);
             exit(0);
         }
         msgrcv(private_queue, &msg ,(size_t) MESSAGE_SIZE,0, MSG_NOERROR);
         printf("%s", msg.value);



 */




}