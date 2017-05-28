//
// Created by przemek on 25.05.17.
//
#include "server.h"
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


void initialize_server();

void parse_args(int argc, char **pString, in_port_t *port, char *path);

void create_task(Task *task, char bfr[50]);

char *stringify_task(Task task, char *str);

void delegate(Task *pTask);


void *network_job(void *arg);

void create_msg(Message *msg, Task *task, enum message_type t);

void add_client(int infd);

in_port_t tcp_port;
char unix_socket_path[MAXUNIXPATH];
pthread_mutex_t clients_array_mutex = PTHREAD_MUTEX_INITIALIZER;
int run = 1;
int CLIENTS = 1;
int task_id = 0;
int clients[MAX_CLIENTS];
char clients_name[MAX_CLIENTS];
int internet_socket;
int local_socket;
int efd;
struct epoll_event events[MAXEVENTS];

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));
    parse_args(argc, argv, &tcp_port, unix_socket_path);
    char bfr[50];
    printf("\n>\t");

    char str[20];
    Task task;
    pthread_t network;
    if (-1 == pthread_create(&network, NULL, network_job, NULL)) {
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

void close_client(int cd) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == cd) {
            int j;
            for (j = i; j < MAX_CLIENTS - 1; j++) {
                clients[j] = clients[j + 1];
            }
            break;

        }
        close(cd);
        CLIENTS--;

    }
}

create_internet_socket() {
    internet_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (internet_socket == -1) {
        perror("error while creating internet socket");
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(tcp_port);


    if (bind(internet_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("error while binding");
        exit(-1);
    }


}

create_local_socket() {
    local_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (local_socket == -1) {
        perror("error while creating local socket");
        exit(-1);
    }

    struct sockaddr_un addr;
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
        printf("a");
        int n, i;
        n = epoll_wait(efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++) {


            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP)) {
                perror("epoll error");
                close_client(events[i].data.fd);
                continue;
            } else if (events[i].data.fd == internet_socket || events[i].data.fd == local_socket) {
                /* We have a notification on the listening socket, which
                                means one or more incoming connections. */
                while (1) {
                    struct sockaddr in_addr;
                    socklen_t in_len = sizeof(in_addr);
                    int infd = accept(events[i].data.fd, &in_addr, &in_len);
                    if (infd == -1) {
                        if ((errno == EAGAIN) ||
                            (errno == EWOULDBLOCK)) {
                            /* We have processed all incoming
                               connections. */
                            break;
                        } else {
                            perror("accept");
                            break;
                        }
                    }
                    if (-1 == make_socket_non_blocking(infd)) {
                        perror("error while making socket non blocking");
                        exit(-1);

                    }

                    add_client(infd);
                }
                continue;
            } else {
                /* We have data on the fd waiting to be read. Read and
             display it. We must read whatever data is available
             completely, as we are running in edge-triggered mode
             and won't get a notification again for the same
             data. */
                int done = 0;

                while (1) {
                    ssize_t count;
                    Message msg;
                    count = read(events[i].data.fd, &msg, sizeof(msg));
                    if (count == -1) {
                        perror("error while reading");
                        exit(-1);
                    }
                    if (count == 0) {
                        perror("end, closing connection");
                        close_client(events[i].data.fd);

                    } else {
                        switch (msg.m_t) {

                            case TASK:
                                break;
                            case PING:
                                break;
                            case PONG:
                                break;
                            case HI:
                                break;
                            case BYE:
                                break;
                            case RESULT:
                                printf("got result");
                                break;
                        }
                    }

                }
            }


        }
    }
}

void add_client(int infd) {
    clients[CLIENTS++] = infd;

}

void *network_job(void *arg) {
    printf("created thread");
    create_internet_socket();
    create_local_socket();
    make_sockets_non_blocking();
    listen_sockets();
    init_epoll();
    work_forever();


}

void delegate(Task *task) {
    int worker;
    pthread_mutex_lock(&clients_array_mutex);
    worker = clients[rand() % CLIENTS];
    pthread_mutex_unlock(&clients_array_mutex);
    Message msg;
    create_msg(&msg, task, TASK);
    if (-1 == send(worker, &msg, sizeof(msg), 0))
        perror("sending message error");

}

void create_msg(Message *msg, Task *task, enum message_type t) {
    msg->task = task;
    msg->m_t = t;
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
            //printf("add task");
            task->op = ADD;
        } else {
            if (strcmp(bfr, "sub") == 0) {
                //printf("sub task");
                task->op = SUB;
            } else {
                if (strcmp(bfr, "mul") == 0) {
                    //printf("mul task");
                    task->op = MUL;
                } else {
                    if (strcmp(bfr, "div") == 0) {
                        //printf("div task");
                        task->op = DIV;
                    } else {
                        if (strcmp(bfr, "end") == 0) {
                            //printf("end task");
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

void clean_up() {
    return;
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


void initialize_server() {
    int socket_d = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_d == -1) {
        perror("socket error");
        exit(-1);
    } else {
        socket_desc = socket_d;
        struct sockaddr_in addr, client;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(8888);
        if (bind(socket_desc, (struct sockaddr *) &addr, sizeof(addr))) {
            perror("bind fail");
            exit(-1);
        };
        puts("bind succes");
        if (listen(socket_desc, 3)) {
            perror("listen fail");
            exit(-1);
        };

        puts("listen succes");
        c = sizeof(struct sockaddr_in);
        puts("waiting for connection..");
        client_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);
        if (client_socket < 0) {
            perror("accept failed");
            exit(-1);
        }
        puts("client connected");
    }

}