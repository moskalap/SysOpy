//
// Created by przemek on 25.05.17.
//
#include "server_prop.h"
#include<arpa/inet.h>
#include <string.h>
#include "sys/un.h"
#include "sys/epoll.h"
#include <zconf.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define MAXEVENTS 64


int socket_desc, client_socket, c;
socklen_t socklen;

void initialize_server();

void parse_args(int argc, char **pString, in_port_t *port, char *path);

void create_task(Task *task, char bfr[50]);

char *stringify_task(Task task, char *str);

void delegate(Task *pTask);

void *network_job(void *arg);

void create_msg(Message *msg, Task *task, enum message_type t);

void add_client(int infd);

void *ping_clients(void *arg);

in_port_t tcp_port;
char unix_socket_path[MAXUNIXPATH];
pthread_mutex_t clients_array_mutex = PTHREAD_MUTEX_INITIALIZER;
int run = 1;
int CLIENTS = 0;
int task_id = 0;
struct sockaddr_storage clients[MAX_CLIENTS];
char *clients_name[MAX_CLIENTS];
int internet_socket;
int local_socket;
int efd;
int soc[MAX_CLIENTS];


struct epoll_event events[MAXEVENTS];

int main(int argc, char *argv[]) {

    srand((unsigned int) time(NULL));
    parse_args(argc, argv, &tcp_port, unix_socket_path);
    char bfr[50];
    printf("\n>\t");
    char str[20];
    Task task;
    pthread_t network;
    pthread_t pinger;

    if (-1 == pthread_create(&network, NULL, network_job, NULL)) {
        perror("error while creating network thread");
        exit(-1);
    }

    if (-1 == pthread_create(&pinger, NULL, ping_clients, NULL)) {
        perror("error while creating network thread");
        exit(-1);
    }

    while (run) {
        task_id++;
        task.task_id = task_id;
        task.op = UNKN;
        scanf("%s", bfr);
        create_task(&task, bfr);
        if (task.op != UNKN) printf("\n<SERVER>%s", stringify_task(task, str));
        printf("\n>\t");
        delegate(&task);
    }


    return 0;
}


void *ping_clients(void *arg) {
    int i = 0;
    Message msg;
    msg.m_t = PING;

    while (run) {
        if (CLIENTS != 0)
            if (i % CLIENTS < CLIENTS) {
                if (-1 == sendto(soc[i % CLIENTS], &msg, sizeof(msg), 0, &clients[i % CLIENTS], socklen)) {
                    close_client(clients[i % CLIENTS]);
                } else {
                    fprintf(stderr, "sent ping to%d\n", i % CLIENTS);
                };
                sleep(5);

            }
        i++;
        sleep(2);

    }

}

void close_client(struct sockaddr_storage adr) {
    fprintf(stderr, "deleting client");
    int i;

    for (i = 0; i < MAX_CLIENTS; i++) {
        if (memcmp(&clients[i], &adr, socklen) == 0) {

        }
        int j;
        for (j = i; j < MAX_CLIENTS - 1; j++) {
            clients[j] = clients[j + 1];

        }
        CLIENTS--;
        return;
    }


}


create_internet_socket() {
    internet_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (internet_socket == -1) {
        perror("error while creating internet socket");
        exit(-1);
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(tcp_port);


    if (bind(internet_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("error while binding");
        exit(-1);
    }


}

create_local_socket() {
    local_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (local_socket == -1) {
        perror("error while creating local socket");
        exit(-1);
    }

    struct sockaddr_un addr;
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, unix_socket_path);


    unlink(unix_socket_path);

    if (bind(local_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("error while creating unix socket");
        exit(-1);

    }
}

int make_socket_non_blocking(int sfd) {
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1) {
        perror("fcntl");
        return -1;
    }

    return 0;
}

void make_sockets_non_blocking() {
    int i, l;
    i = make_socket_non_blocking(internet_socket);
    l = make_socket_non_blocking(local_socket);
    if (i == -1 || l == -1) {
        perror("error while making sockets non blocking");
        exit(-1);
    }
}

void listen_sockets() {
    int i, l;
    i = listen(internet_socket, SOMAXCONN);
    l = listen(local_socket, SOMAXCONN);
    if (i == -1 || l == -1) {
        perror("error while listening sockets");
        exit(-1);
    }
}

void init_epoll() {
    efd = epoll_create1(0);
    if (efd == -1) {
        perror("creating epoll");
        exit(-1);
    }
    struct epoll_event ev;
    ev.data.fd = internet_socket;
    ev.events = EPOLLIN | EPOLLRDHUP;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, internet_socket, &ev) == -1) {
        perror("error while adding internet socket to epoll");
        exit(-1);
    }
    ev.data.fd = local_socket;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, local_socket, &ev) == -1) {
        perror("error while adding local socket to epoll");
        exit(-1);

    }

}

void work_forever() {
    while (run) {

        int n, i;
        n = epoll_wait(efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++) {

            if ((events[i].events & EPOLLERR) != 0 && (events[i].events & EPOLLHUP) != 0) {
                perror("epoll error or closed socket");

                continue;
            } else {

                while (1) {
                    Message msg;
                    struct sockaddr_storage from;
                    memset(&from, 0, sizeof(from));
                    socklen = sizeof(struct sockaddr_storage);


                    if (-1 == recvfrom(events[i].data.fd, &msg, sizeof(msg), MSG_WAITALL, (struct sockaddr *) &from,
                                       &socklen))
                        perror("recv ");
                    else

                        procces_message(msg, from, i);

                    break;

                }
            }

        }
    }
}

void procces_message(Message msg, struct sockaddr_storage from, int i) {
    switch (msg.m_t) {

        case TASK:
            fprintf(stderr, "got task from client, smth wrong..\n");

            break;
        case PING:
            perror("ping");
            break;
        case PONG:
            perror("pong");
            break;
        case HI:
            fprintf(stderr, "username %s trying to log in\n", msg.name);
            if (CLIENTS < MAX_CLIENTS) {
                for (int c = 0; c < CLIENTS; c++) {
                    if (strncmp(&clients_name[c], &msg.name, MAX_NAME_LEN) == 0) {
                        fprintf(stderr, "exist!\n");
                        strncpy(msg.name, "USER EXIST", MAX_NAME_LEN);

                        msg.m_t = BYE;
                        if (-1 ==
                            sendto(events[i].data.fd, &msg, sizeof(msg), 0, (const struct sockaddr *) &from,
                                   socklen))
                            perror("sento ");
                        return;

                    };

                }


                if (-1 == sendto(events[i].data.fd, &msg, sizeof(msg), 0, (const struct sockaddr *) &from, socklen))
                    perror("sento ");
                soc[CLIENTS] = events[i].data.fd;
                memcpy(&clients_name[CLIENTS], &msg.name, MAX_NAME_LEN);
                memcpy(&clients[CLIENTS++], &from, sizeof(from));

            } else {
                strncpy(msg.name, "NO SLOTS", MAX_NAME_LEN);
                fprintf(stderr, "maximum clients exceded, sending goodbye\n");
                msg.m_t = BYE;
                if (-1 ==
                    sendto(events[i].data.fd, &msg, sizeof(msg), 0, (const struct sockaddr *) &from,
                           socklen))
                    perror("sento ");

            };
            break;
        case BYE:
            break;
        case RESULT:
            fprintf(stderr, "got result <%d>%d from %s", msg.task.task_id, msg.task.result,
                    msg.name);

            break;
    }
}


void *network_job(void *arg) {
    printf("created thread");
    create_internet_socket();
    create_local_socket();
    make_sockets_non_blocking();
    init_epoll();
    work_forever();


}

void delegate(Task *task) {
    struct sockaddr_storage worker;
    pthread_mutex_lock(&clients_array_mutex);
    if (CLIENTS == 0) {
        fprintf(stderr, "no clients!\n");
        pthread_mutex_unlock(&clients_array_mutex);
        return;
    }
    int k = rand() % CLIENTS;

    worker = clients[k];
    pthread_mutex_unlock(&clients_array_mutex);
    Message msg;
    create_msg(&msg, task, TASK);
    if (-1 == sendto(soc[k], &msg, sizeof(msg), 0, (const struct sockaddr *) &worker, socklen))
        perror("sending message error");

}

void create_msg(Message *msg, Task *task, enum message_type t) {
    msg->task.operand1 = task->operand1;
    msg->task.operand2 = task->operand2;
    msg->task.op = task->op;
    msg->m_t = t;
    msg->task.task_id = task->task_id;
}

char *stringify_task(Task task, char *str) {

    if (task.op == ADD)
        sprintf(str, "t_id<%d>\t%d + %d", task.task_id, task.operand1, task.operand2);
    if (task.op == SUB)
        sprintf(str, "t_id<%d>\t%d - %d", task.task_id, task.operand1, task.operand2);
    if (task.op == MUL)
        sprintf(str, "t_id<%d>\t%d * %d", task.task_id, task.operand1, task.operand2);
    if (task.op == DIV)
        sprintf(str, "t_id<%d>\t%d / %d", task.task_id, task.operand1, task.operand2);

    return str;
}

void create_task(Task *task, char bfr[50]) {
    if (strcmp("help", bfr) == 0) {
        printf("helping msg");
        task->op = UNKN;
        return;
    } else {
        if (strcmp(bfr, "add") == 0) {

            task->op = ADD;
        } else {
            if (strcmp(bfr, "sub") == 0) {

                task->op = SUB;
            } else {
                if (strcmp(bfr, "mul") == 0) {

                    task->op = MUL;
                } else {
                    if (strcmp(bfr, "div") == 0) {

                        task->op = DIV;
                    } else {
                        if (strcmp(bfr, "end") == 0) {

                            task->op = END;
                        } else {
                            task->op = UNKN;
                        }

                    }
                }
            }
        }
    }
    if (task->op != UNKN) {
        scanf("%d", &(task->operand1));
        scanf("%d", &(task->operand2));
    }


}


void parse_args(int argc, char *pString[], in_port_t *port, char *path) {
    if (argc != 3) {
        printf("usage: server TCP_PORT UNIX_PATH\n");
        exit(-1);
    } else {
        int intport = atoi(pString[1]);
        tcp_port = (in_port_t) intport;
        strncpy(unix_socket_path, pString[2], MAXUNIXPATH);
        unix_socket_path[MAXUNIXPATH - 1] = '\0';
        printf("Running server on port %d and path %s\n", intport, unix_socket_path);

    }


};

