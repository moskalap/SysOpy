//
// Created by przemek on 30.05.17.
//

#ifndef COUNT_SERVER_DG_SERVER_PROP_H
#define COUNT_SERVER_DG_SERVER_PROP_H

#include <stdio.h>

#define MAXUNIXPATH 108
#define MAX_NAME_LEN 8
#define MAX_CLIENTS 16

#include "stdlib.h"

#include <sys/socket.h>

enum operation {
    ADD, SUB, MUL, DIV, END, UNKN

};
enum message_type {
    TASK, PING, PONG, HI, BYE, RESULT,
} message_type;
typedef struct Task {
    int task_id;
    enum operation op;
    int operand1;
    int operand2;
    int result;

} Task;
typedef struct Result {
    int task_id;
    int result;
} Result;
typedef struct Message {
    enum message_type m_t;
    Task task;
    char name[MAX_NAME_LEN];
} Message;


#endif //COUNT_SERVER_DG_SERVER_PROP_H
