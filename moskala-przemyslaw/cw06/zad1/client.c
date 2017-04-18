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
pid_t login;
pid_t server=0;
Request_Type get_type(char msg[]){
    if(msg[0] == ':'){
        switch (msg[1]){
            case 'e':
                return echo;
            case 't':
                return time;
            case 'q':
                run=0;
                return termination;
            case 'u':
                return to_upper;
            default:
                return undefined;
        }
    }
    return undefined;

}
char* str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return str;
}
Message * create_message(Request_Type type, char message[]){
    Message * m = malloc(sizeof(Message));
    strcpy(m->value, message);
    m->sender=getpid();
    m->type=type;
}

void display_message(Message* msg){
    printf("From: %d\nType: %s\nVal: %s\n",msg->sender,
           (msg->type == echo)?"echo":((msg->type == to_upper)?"to_upper":(msg->type == time)?"time":(msg->type == termination)?"term":"noth"), msg->value);

}
void send_message(Message * msg, int q){
    msgsnd(q, msg,MSG_LEN_MAX,0);
}
int main(){
    display_message(create_message(echo, "blalabla"));
    printf(getenv("HOME"));

    char *buffer;
    size_t bufsize = 32;
    size_t characters;
    login=getpid();

    buffer = (char *)malloc(bufsize * sizeof(char));
    Message * msg;
    char * b;
    Request_Type type;
    while(run){

        printf("\n%d@%d:$ ",login,server);
        getline(&buffer,&bufsize,stdin);
        type=get_type(buffer);
        b = str_cut(buffer,0,2);
        if(type != undefined)
            send_message(create_message(type,b));
        //        display_message(create_message(type,b));
    }

}