
#include <zconf.h>
#include <time.h>
#include "pcommon.h"


int run = 1;
int myID = 0;
mqd_t qd_server, qd_client;
generate_name(char *s) {
    const char charset[] = "abcdefghijklmnopqrstuvwxy";
    int i;
    for (i = 0; i < MAX_LEN_NAMEQ; i++) {
        int key = rand() % (int) (sizeof charset - 1);
        s[i] = charset[key];
    }
    s[i - 1] = '\0';
    s[0] = '/';

}


void message_send(mqd_t queue, Message *msg) {
    if (-1 == (mq_send(queue, (char *) msg, MESSAGE_SIZE, 0))) {
        perror("error while sending a message");
    }
}

void initalize(char *a) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MSGS_LIMIT;
    attr.mq_msgsize = MESSAGE_SIZE;
    if ((qd_client = mq_open(a, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror("Client: mq_open (client)");
        exit(1);
    }

    if ((qd_server = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        perror("Client: mq_open (server)");
        exit(1);
    }
}

char get_type(char msg[]) {
    if (msg[0] == ':') {
        switch (msg[1]) {
            case 'e':
                return ECHO;
            case 't':
                return TIME_REQ;
            case 'q':
                run = 0;
                return TERM;
            case 'u':
                return TO_UPPER;
            default:
                return -1;
        }
    }
    return -1;

}

char *str_cut(char *str, int begin, int len) {
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return str;
}

Message *message_get(mqd_t queue, Message *msg) {
    if (-1 == (mq_receive(queue, (char *) msg, MESSAGE_SIZE, 0))) {
        printf(stderr, "error while receiving");
        exit(-11);
    }
    return msg;
}

void close_connection(char *a) {
    if (mq_close(qd_client) == -1) {
        perror("Client: mq_close");
        exit(1);
    }

    if (mq_unlink(a) == -1) {
        perror("Client: mq_unlink");
        exit(1);
    }
    printf("Client: bye\n");
}

int main(int argc, char **argv) {
    srand(time(NULL));

    char client_queue_name[MAX_LEN_NAMEQ];
    generate_name(client_queue_name);

    initalize(client_queue_name);


    Message m;
    m.from = getpid();
    strcpy(m.val, client_queue_name);
    m.mtype = LOGIN;

    message_send(qd_server, &m);
    message_get(qd_client, &m);


    if (strcmp(m.val, ":(") == 0) {
        printf("error: maximum users exceeded.\n");
        exit(0);
    }

    myID = atoi(m.val);
    printf("Recieved id %d", myID);
    printf("Connection successful\n"
                   "use \n"
                   ":e TEXT\t\tfor echo\n"
                   ":u TEXT\t\tfor translate to upper case\n"
                   ":t\t\t\tto receive time\n"
                   ":q\t\t\tfor terminate server \n"
    );
    

    int type;
    char *buffer;
    size_t bufsize = 32;
    size_t characters;
    char *b;

    buffer = (char *) malloc(bufsize * sizeof(char));
    while (run) {
        printf("\n%d@serv: >\t ", myID);
        getline(&buffer, &bufsize, stdin);
        type = get_type(buffer);
        b = str_cut(buffer, 0, 2);
        m.mtype = type;
        strcpy(m.val, b);
        m.from = myID;
        message_send(qd_server, &m);
        if (type == TERM) {

            exit(0);
        }
        message_get(qd_client, &m);
        if (m.mtype == TERM) {

            exit(0);
        }
        printf(m.val);


    }


    close_connection(client_queue_name);



    exit(0);
}
