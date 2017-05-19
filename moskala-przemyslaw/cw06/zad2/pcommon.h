#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/SERVER-test"
#define QUEUE_PERMISSIONS 0666

#define MAX_MSG_SIZE 35
#define MAX_LEN_NAMEQ 8
#define MAX_MSG_LEN 64
#define MSGS_LIMIT 10
#define MAX_USERS 9


#define LOGIN 1
#define ECHO 2
#define TO_UPPER 3
#define TIME_REQ 4
#define TERM 5


typedef struct Message {
    char mtype;
    pid_t from;
    char val[MAX_MSG_LEN];
} Message;
static const int MESSAGE_SIZE = sizeof(Message);
