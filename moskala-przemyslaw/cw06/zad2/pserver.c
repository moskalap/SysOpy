
#include <ctype.h>
#include <time.h>
#include "pcommon.h"

int run = 1;
char *str_cut(char *str, int begin, int len) {
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return str;
}

char *convert_to_upper(char *msg) {
    int i = 0;
    while (msg[i]) {
        msg[i] = toupper(msg[i]);
        i++;
    }
    return msg;
}

int users[9];
int nextid = -1;


mqd_t initialize_server() {

    mqd_t q;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MSGS_LIMIT;
    attr.mq_msgsize = MESSAGE_SIZE;

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
    printf("opened suc");
    return q;


}

char *get_time() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);

}

void send_msg(mqd_t queue, Message *msg) {
    if (-1 == (mq_send(queue, (char *) msg, MESSAGE_SIZE, 0))) {
        perror("err");
    }
}

void process_mesage(Message *m) {


    switch (m->mtype) {
        case LOGIN:
            if (nextid > MAX_USERS) {
                mqd_t q = open_client_queue(m->val);
                strcpy(m->val, ":(");
                send_msg(q, m);

            }
            users[++nextid] = open_client_queue(m->val);
            char b[5];
            sprintf(b, "%d", nextid);
            strcpy(m->val, b);
            send_msg(users[nextid], m);
            break;
        case TO_UPPER:
            strcpy(m->val, convert_to_upper(m->val));
            send_msg(users[m->from], m);
            break;
        case TIME_REQ:

            strcpy(m->val, get_time());
            send_msg(users[m->from], m);
            break;
        case TERM:
            for (int i = 0; i <= nextid; i++) {
                if (i != m->from) send_msg(i, m);
            }
            run = 0;
        case ECHO:
            send_msg(users[m->from], m);
            break;
        default:
            break;

    }

}

Message *message_get(mqd_t queue, Message *msg) {
    if (-1 == (mq_receive(queue, (char *) msg, MESSAGE_SIZE, 0))) {
        fprintf(stderr, "err while receiv");
        exit(-11);
    }
    return msg;
}

void close_connection(mqd_t q) {
    if (mq_close(q) == -1) {
        perror("Server: mq_close");
        exit(1);
    }

}

int main(int argc, char **argv) {
    mqd_t public_queue, qd_client;   // queue descriptors
    long token_number = 1; // next token to be given to client
    public_queue = initialize_server();

    Message m;
    int nonempty = 1;
    struct mq_attr attr;

    while (run || nonempty) {
        message_get(public_queue, &m);
        printf("Server: message received.\n");
        process_mesage(&m);
        mq_getattr(public_queue, &attr);
        nonempty = attr.mq_curmsgs;

    }

    for (int j = 0; j <= nextid; j++) {
        close_connection(users[j]);
    }

    if (mq_close(public_queue) == -1) {
        perror("Serrver: mq_close");
        exit(1);
    }

    if (mq_unlink(SERVER_QUEUE_NAME) == -1) {
        perror("Server: mq_unlink");
        exit(1);
    }
}
