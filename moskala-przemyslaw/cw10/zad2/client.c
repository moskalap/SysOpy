#include <zconf.h>
#include <string.h>
#include <netinet/in.h>

#include "sys/un.h"
#include "arpa/inet.h"
#include "server_prop.h"

void init_unix_connection();

void init_internet_connection();

void parse_args(int argc, char *argv[]);

int socket_fd;

void try_log_in();

int local_connection;
char unix_path[MAXUNIXPATH];
char client_name[MAX_NAME_LEN];
char *ip_address;
in_port_t port;

void send_result(int res, Task task);

int main(int argc, char *argv[]) {
    parse_args(argc, argv);
    if (local_connection)
        init_unix_connection();
    else
        init_internet_connection();
    printf("trying do log in\n");
    try_log_in();


    while (1) {
        Message msg;
        ssize_t count = recv(socket_fd, &msg, sizeof(msg), MSG_WAITALL);

        fprintf(stderr, "got msg: ");
        switch (msg.m_t) {

            case TASK:
                fprintf(stderr, "task\n");
                int a = msg.task.operand1;
                int b = msg.task.operand2;
                switch (msg.task.op) {

                    case ADD:
                        send_result(a + b, msg.task);
                        break;

                    case SUB:
                        send_result(a - b, msg.task);
                        break;
                    case MUL:
                        send_result(a * b, msg.task);
                        break;
                    case DIV:
                        send_result(a / b, msg.task);
                        break;
                    case END:
                        break;
                    case UNKN:
                        break;
                }
                break;
            case PING:
                msg.m_t = PONG;
                strncpy(msg.name, client_name, MAX_NAME_LEN);
                fprintf(stderr, "ping\n");
                if (-1 == send(socket_fd, &msg, sizeof(msg), 0))
                    perror("error while ping");
                break;

            case PONG:
                fprintf(stderr, "pong\n");
                //?//
                break;
            case HI:

                fprintf(stderr, "hi\n");
                break;
            case BYE:
                shutdown(socket_fd, SHUT_RDWR);
                close(socket_fd);
                fprintf(stderr, "bye");
                exit(0);
            case RESULT:
                //?//
                break;
        }


    }
    return 0;
}

void send_result(int res, Task task) {
    Message msg;
    msg.task = task;
    msg.m_t = RESULT;
    msg.task.result = res;
    strncpy(msg.name, client_name, MAX_NAME_LEN);
    fprintf(stderr, "sending result...\n");
    if (0 != send(socket_fd, &msg, sizeof(msg), 0))
        perror(" error send result");
}

void try_log_in() {
    Message msg;
    Message msg2;
    msg2.m_t = BYE;
    msg.m_t = HI;
    strncpy(msg.name, client_name, MAX_NAME_LEN);
    if (-1 == sendto(socket_fd, &msg, sizeof(msg), MSG_WAITALL, NULL, NULL)) {
        perror("send");

    };

    if (-1 == recv(socket_fd, &msg2, sizeof(msg2), MSG_WAITALL)) {
        perror("error while receiving");
        exit(-1);
    };

    if (msg2.m_t == BYE) {
        fprintf(stderr, "%s", msg2.name);
        exit(-1);


    }
    if (msg2.m_t == HI) {
        fprintf(stderr, "hurra");
    }


}

void init_unix_connection() {
    socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        perror("error while creating socket");
        exit(-1);
    }
    struct sockaddr_un addr;
    struct sockaddr_un cliaddr;
    addr.sun_family = AF_UNIX;
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));


    strncpy(addr.sun_path, unix_path, MAXUNIXPATH);

    if (-1 == bind(socket_fd, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)))
        perror("aaa");

    connect(socket_fd, (const struct sockaddr *) &addr, sizeof(addr));

}

void init_internet_connection() {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1) {
        perror("error while creating socket");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    if (inet_aton(ip_address, &addr.sin_addr) == 0) {
        perror("wrong ip");
        exit(-1);
    }


    if (connect(socket_fd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("error while connecting");
        exit(-1);
    }

}

void parse_args(int argc, char *argv[]) {
    if (argc != 5 && argc != 4) {
        printf("usage: client CLIENT_NAME CONNECTION_TYPE DESTINATION\n"
                       "CONNECTION_TYPE:\n"
                       "\tu\tfor local connectin(unix_socket)\n"
                       "\ti\tfor internet connection\n"
                       "DESTINATION:\n"
                       "\tunix path\tfor local connectin(unix_socket)\n"
                       "\tip port\tfor internet connection\n");
        exit(-1);
    }

    strcpy(client_name, argv[1]);
    if (strcmp(argv[2], "u") == 0) {
        local_connection = 1;

    } else {
        if (strcmp(argv[2], "i") == 0) {
            local_connection = 0;
        } else {
            perror("wrong connection type");
            exit(-1);
        }
    }
    if (local_connection) {
        strcpy(unix_path, argv[3]);
        printf("name %s local unix %s", client_name, unix_path);
    } else {
        ip_address = argv[3];
        port = (in_port_t) atoi(argv[4]);

        printf("name %s internet ip %s port %d", client_name, ip_address, port);
    }

}