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

const char PATH[] = "HOME";
const int PROJ_ID = 101;


#define LOGIN 'a'
#define ECHO 'b'
#define TO_UPPER 'c'
#define TIME_REQ 'd'
#define TERM 'e'
#define MSGS_MAX 10
#define MSG_SIZE 75
#define MSG_LEN_MAX 75
#define USERS_MAX 10
#define MAX_NAME_L 16
typedef struct Message {
    char type;
    pid_t sender;
    char value[MSG_SIZE];

} Message;
static const char *SERVER_NAME = "/tejst";
const int MESSAGE_SIZE = sizeof(Message);
#endif //CW05Z02MASTER_COMMON_H
