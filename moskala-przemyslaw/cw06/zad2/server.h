//
// Created by przemek on 18.04.17.
//

#ifndef CW05Z02MASTER_SERVER_H
#define CW05Z02MASTER_SERVER_H

#include "common.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct Node {
    struct Node *prev;
    pid_t user_pid;
    int q_id;
    int id;
    struct Node *next;

} Node;
typedef struct List {
    int n;
    int q_ids[USERS_MAX];
    int is_taken[USERS_MAX];

    Node *head;
    Node *last;


} List;
#endif //CW05Z02MASTER_SERVER_H
