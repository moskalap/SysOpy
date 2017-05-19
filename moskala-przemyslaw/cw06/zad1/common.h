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

const char PATH[] = "HOME";
const int PROJ_ID = 101;


#define LOGIN 1
#define ECHO 2
#define TO_UPPER 3
#define TIME_REQ 4
#define TERM 5


#define MSG_LEN_MAX 75
#define USERS_MAX 10
typedef struct Message{
    long type;
    pid_t sender;
    char value[MSG_LEN_MAX];

}Message;
const int MESSAGE_SIZE = sizeof(Message) - sizeof(long);
#endif //CW05Z02MASTER_COMMON_H
