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
int main(){
    display_message(create_message(echo, "blalabla"));
    printf(getenv("HOME"));

    char *buffer;
    size_t bufsize = 32;
    size_t characters;
    login=getpid();

    buffer = (char *)malloc(bufsize * sizeof(char));
    while(run){
        printf("\n%d@%d:$ ",login,server);
        getline(&buffer,&bufsize,stdin);
        display_message(create_message(get_type(buffer),buffer));
    }

}