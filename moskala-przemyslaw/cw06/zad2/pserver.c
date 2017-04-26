
#include "pcommon.h"


char *str_cut(char *str, int begin, int len) {
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return str;
}

int users[9];
int nextid = -1;


mqd_t initialize_server() {
    struct mq_attr attr;
    mqd_t q;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((q = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror("Server: mq_open (server)");
        exit(1);
    }
    return q;
}

mqd_t open_client_queue(char *name) {
    mqd_t q;
    if ((q = mq_open(name, O_WRONLY)) == 1) {
        printf(stderr, "error open clieny");
        exit(-1);
    }
    return q;


}

void process_mesage(char *in_buffer) {
    int type = in_buffer[0] - '0';
    printf(in_buffer);
    int id = in_buffer[2] - '0';

    switch (type) {
        case LOGIN:
            printf("loggin attempt");
            in_buffer = str_cut(in_buffer, 0, 2);
            users[++nextid] = open_client_queue(in_buffer);
            break;
        case TO_UPPER:
            printf("toup");
            break;
        case TIME_REQ:
            printf("time");
            break;
        case TERM:
            printf("term");
            break;
        default:
            break;

    }

}

int main(int argc, char **argv) {
    mqd_t public_queue, qd_client;   // queue descriptors
    long token_number = 1; // next token to be given to client
    public_queue = initialize_server();

    char in_buffer[MSG_BUFFER_SIZE];
    char out_buffer[MSG_BUFFER_SIZE];
    Message m;
    size_t s = sizeof(m);

    while (1) {

        if (mq_receive(public_queue, (char *) &m, sizeof(m), NULL) == -1) {
            perror("error while receiviing msg");
            exit(1);
        }


        printf("Server: message received.\n");
        process_mesage(in_buffer);

/*
        if ((qd_client = mq_open (in_buffer, O_WRONLY)) == 1) {
            perror ("Server: Not able to open client queue");
            continue;
        }
        char msg[] = "msg";
        sprintf (out_buffer, "%ld:%s", token_number, msg);

        if (mq_send (qd_client, out_buffer, strlen (out_buffer), 0) == -1) {
            perror ("Server: Not able to send message to client");
            continue;
        }

        printf ("Server: response sent to client.\n");
        token_number++;
        */
    }
}
