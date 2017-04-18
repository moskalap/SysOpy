//
// Created by przemek on 18.04.17.
//

#ifndef CW05Z02MASTER_COMMON_H
#define CW05Z02MASTER_COMMON_H
#include <stdlib.h>
#include "stdio.h"
#include <string.h>
#include <zconf.h>
#include <printf.h>
#include "client.h"
typedef enum Request_Type{
echo, to_upper, time, termination, undefined
}Request_Type;
#define MSG_LEN_MAX 64
const int USERS_MAX = 10;
typedef struct Message{
    Request_Type type;
    char value[MSG_LEN_MAX];
    pid_t sender;
}Message;
#endif //CW05Z02MASTER_COMMON_H
