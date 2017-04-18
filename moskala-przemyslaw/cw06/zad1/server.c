//
// Created by przemek on 18.04.17.
//

#include "server.h"
int run=1;
int initialize_server(key_t key, int flags){
    int q =msgget(key,flags);
    if (q == -1){
        fprintf(stderr, "error while creating queue.\n");
        exit(-1);
    }
    return q;
}
void terminate_server(){
    exit(0);
}
void queue_get(int q, Message* msg, int MSGLEN){
    msgrcv(q,msg,MSGLEN,0,0);

}
void process(Message * msg){
    switch(msg->type){
        case echo:
            send_message(msg->value, msg->sender);
            break;
        case time:
            send_message()
            break;
        case to_upper:
            break;
        case termination:
            break;
        default:
            break;

    }

}
int main(){
    key_t key;
    int flags;
    int queue = initialize_server(key,flags);
    Message * msg;
    while(run){
        queue_get(queue, msg, MSG_LEN_MAX);
        if(msg) process(msg);
    }
    terminate_server();
}