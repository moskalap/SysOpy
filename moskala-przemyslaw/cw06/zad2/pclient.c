
#include <zconf.h>
#include "pcommon.h"

int run = 1;
int myID = 0;
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

int get_type(char msg[]) {
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


int main(int argc, char **argv) {
    char client_queue_name[MAX_LEN_NAMEQ];
    generate_name(client_queue_name);


    mqd_t qd_server, qd_client;   // queue descriptors




    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_client = mq_open(client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror("Client: mq_open (client)");
        exit(1);
    }

    if ((qd_server = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        perror("Client: mq_open (server)");
        exit(1);
    }

    char in_buffer[MSG_BUFFER_SIZE];

    printf("Ask for a token (Press <ENTER>): ");

    char temp_buf[40];

    char to_send[40] = "";
    char bfr[2];
    sprintf(bfr, "%d:", LOGIN);
    strcat(to_send, bfr);
    strcat(to_send, client_queue_name);
    // sprintf(in_buffer,"%d:%s", LOGIN, client_queue_name);

    Message m;
    m.type = LOGIN;
    m.sender = getpid();
    strcpy(m.value, client_queue_name);

    int r;
    r = mq_send(qd_server, (char *) &m, sizeof(m), 0);

    if (r == -1) {
        perror("Client: Not able to send message to server");

    }
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
        sprintf(in_buffer, "%d:%d:%s", type, myID, b);

        if (mq_send(qd_server, in_buffer, strlen(in_buffer), 0) == -1) {
            perror("Client: Not able to send message to server");

        }


        if (type == TERM) {

            exit(0);
        }


/*

        if (mq_receive (qd_client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Client: mq_receive");
            exit (1);
        }
        // display token received from server
        printf ("Client: Token received from server: %s\n\n", in_buffer);

        printf ("Ask for a token (Press ): ");
        */
    }


    if (mq_close(qd_client) == -1) {
        perror("Client: mq_close");
        exit(1);
    }

    if (mq_unlink(client_queue_name) == -1) {
        perror("Client: mq_unlink");
        exit(1);
    }
    printf("Client: bye\n");

    exit(0);
}
