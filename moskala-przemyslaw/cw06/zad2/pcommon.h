#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/sp-example-servenr"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 35
#define MAX_MSG_SIZE 35
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define MAX_LEN_NAMEQ 8

typedef struct Message {
    long type;
    pid_t sender;
    char value[20];

} Message;


#define LOGIN 1
#define ECHO 2
#define TO_UPPER 3
#define TIME_REQ 4
#define TERM 5