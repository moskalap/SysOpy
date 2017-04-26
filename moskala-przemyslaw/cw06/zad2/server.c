//
// Created by przemek on 18.04.17.
//

#include "server.h"
#include "common.h"
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <mqueue.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>


int run = 1;
List *users;


List *create_list() {
    List *l = malloc(sizeof(List));
    int i = 0;
    for (i; i < USERS_MAX; i++) {
        l->is_taken[i] = 0;
        l->q_ids[i] = 0;
    }

    l->head = NULL;
    l->last = NULL;
    l->n = 0;

    return l;

}

int add_user(List *list, pid_t client_pid, int q_id) {
    if (list->n > USERS_MAX) {
        fprintf(stderr, "cannot connect another user, due to reach amount of limit\n");
        return -1;
    } else {

        Node *user = malloc(sizeof(Node));
        user->q_id = q_id;
        user->user_pid = client_pid;
        user->next = NULL;
        user->prev = NULL;

        int i = 0;

        for (i; i < USERS_MAX; i++) {
            if (list->is_taken[i] == 0) break;
        }
        user->id = i;
        list->q_ids[i] = q_id;
        list->is_taken[i] = 1;


        if (list->n == 0) {
            list->head = user;
            list->last = user;
            list->n++;
            return i;
        } else {
            user->prev = list->last;
            list->last->next = user;
            list->n++;
            return i;
        }

    }
}

void delete_user(List *list, pid_t pid) {

    //TODO
}

int initialize_server(key_t key, int flags) {
    int q = 10;
    q = msgget(key, flags);
    if (q == -1) {
        fprintf(stderr, "error while creating queue.\n");
        exit(-1);
    }
    return q;
}

mqd_t create_public_queue() {
    mqd_t q;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MSGS_MAX;
    attr.mq_msgsize = MSG_SIZE;
    q = mq_open(SERVER_NAME, O_CREAT | O_RDONLY, 0666, &attr);
    if (q == -1) {
        fprintf(stderr, "error while creating a public queue\n");
        exit(-1);
    }
    return q;

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


char *get_time() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);

}

void close_queue(mqd_t q) {
    if (-1 == (mq_close(q))) {
        fprintf(stderr, "%error while closing queue\n");
        exit(1);
    }
    if (-1 == (mq_unlink(SERVER_NAME))) {
        fprintf(stderr, "%error while closing queue\n");
        exit(1);
    }
}

void terminate_server() {
    exit(0);
}

Message *queue_get(mqd_t q, Message *msg, int MSGLEN) {


    if (-1 == mq_receive(q, (char *) msg, MESSAGE_SIZE, 0)) {
        printf(stderr, "error while receiving msg\n");
        exit(-1);
    }

    return msg;
}

char *convert_to_upper(char *msg) {
    int i = 0;
    while (msg[i]) {
        msg[i] = toupper(msg[i]);
        i++;
    }
    return msg;
}

void send_message(char *value, int q_id, long type) {
    Message msg;
    strcpy(msg.value, value);
    msg.type = type;
    msg.sender = -1;//SERVER

    if (msgsnd(q_id, &msg, MESSAGE_SIZE, 0) == -1)
        fprintf(stderr, "error while sending a message %s\n");
}


void process(Message *msg) {
    if (msg)
        switch (msg->type) {
            case LOGIN:
                printf("login attempt from %d\n", msg->sender);
                //display_message(msg);
                int is_added = add_user(users, msg->sender, atoi(msg->value));
                char id[20];
                sprintf(id, "%d", is_added);
                send_message(is_added == -1 ? ":(" : id, atoi(msg->value), LOGIN);

                break;
            case ECHO:
                send_message(msg->value, users->q_ids[msg->sender], ECHO);
                break;
            case TIME_REQ:
                send_message(get_time(), users->q_ids[msg->sender], TIME_REQ);
                break;
            case TO_UPPER:
                send_message(convert_to_upper(msg->value), users->q_ids[msg->sender], TO_UPPER);
                break;
            case TERM:
                run = 0;

            default:
                break;

        }

}

int are_tasks(int q) {
    return 0;
}

int main() {
    mqd_t queue = create_public_queue();


    users = create_list();

    Message msg;
    while (run || are_tasks(queue) == 1) {
        process(queue_get(queue, &msg, MESSAGE_SIZE));

    }

    close_queue(queue);
/*
    key_t key = get_key();
    Message msg;
   while (run || are_tasks(queue) == 1){
       process(queue_get(queue, &msg, MESSAGE_SIZE));

   }

    close_queue(queue);

 */

}