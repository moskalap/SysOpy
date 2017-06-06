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

void incoming_msg_handler(int i);

void incoming_client_handler(int i);

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
int CLIENTS = 0;
int task_id = 0;
int clients[MAX_CLIENTS];
char *clients_name[MAX_CLIENTS];
int internet_socket;
int local_socket;
int efd;

void *ping_clients(void *arg);

struct epoll_event events[MAXEVENTS];

int main(int argc, char *argv[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        char strl[MAX_NAME_LEN];
        clients_name[i] = strl;
    }

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
        if (task.op == END) {
            Message msg;
            msg.m_t = BYE;
            for (int i = 0; i < CLIENTS; i++)
                send(clients[i], &msg, sizeof(msg), MSG_WAITALL);
            shutdown(internet_socket, SHUT_RDWR);
            shutdown(local_socket, SHUT_RDWR);
            close(internet_socket);
            close(local_socket);
            exit(0);
        }
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
                if (-1 == send(clients[i % CLIENTS], &msg, sizeof(msg), 0)) {

                } else {
                    fprintf(stderr, "sent ping to %s\n", clients_name[i % CLIENTS]);
                };
                sleep(5);
            }
        i++;
        sleep(5);


    }
}

void close_client(int cd) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == cd) {
            int j;
            for (j = i; j < MAX_CLIENTS - 1; j++) {
                clients[j] = clients[j + 1];
            }

            CLIENTS--;
            break;
        }
        close(cd);


    }
}

void create_internet_socket() {
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

void create_local_socket() {
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

void work() {
    while (run) {

        int n, i;
        n = epoll_wait(efd, events, MAXEVENTS, -1);

        for (i = 0; i < n; i++) {

            if ((events[i].events & EPOLLERR) != 0 || (events[i].events & EPOLLHUP) != 0) {
                perror("epoll error - client disconnected");
                close_client(events[i].data.fd);
                continue;
            } else if (events[i].data.fd == internet_socket || events[i].data.fd == local_socket) {

                incoming_client_handler(i);
                continue;
            } else {
                /* We have data on the fd waiting to be read. Read and
             display it. We must read whatever data is available
             completely, as we are running in edge-triggered mode
             and won't get a notification again for the same
             data. */

                incoming_msg_handler(i);
            }


        }
    }
}

void incoming_msg_handler(int i) {
    while (1) {
        ssize_t count;
        Message msg;
        count = recv(events[i].data.fd, &msg, sizeof(msg), MSG_WAITALL);
        if (count == -1) {
            perror("error while reading");
            close_client(events[i].data.fd);
            //exit(-1);
        }
        if (count == 0) {
            perror("end, closing connection");
            close_client(events[i].data.fd);

        } else {
            switch (msg.m_t) {

                case TASK:
                    perror("task");
                    break;
                case PING:
                    perror("ping");
                    break;
                case PONG:
                    fprintf(stderr, "pong from %s\n", msg.name);
                    break;
                case HI:
                    perror("hi");
                    break;
                case BYE:

                    break;
                case RESULT:
                    fprintf(stdout, "\ngot result <%d>%d from %s\n", msg.task.task_id, msg.task.result, msg.name);

                    break;
            }
        }
        break;

    }
}

void incoming_client_handler(int i) {


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
        Message msg;
        msg.m_t = BYE;
        if (-1 != recv(infd, &msg, sizeof(msg), MSG_WAITALL))
            fprintf(stderr, "request from %s\n", msg.name);
        if (CLIENTS >= MAX_CLIENTS) {
            msg.m_t = BYE;
            send(infd, &msg, sizeof(msg), MSG_WAITALL);
            close(infd);
            return;


        } else {
            int j = 0;
            for (j = 0; j < CLIENTS; j++) {
                if (strncmp(clients_name[j], msg.name, MAX_NAME_LEN) == 0) {
                    msg.m_t = BYE;
                    send(infd, &msg, sizeof(msg), MSG_WAITALL);
                    close(infd);
                    return;
                }
            }
            msg.m_t = HI;
            struct epoll_event lis_event;
            lis_event.events = EPOLLIN | EPOLLET;
            lis_event.data.fd = infd;
            if (epoll_ctl(efd, EPOLL_CTL_ADD, infd, &lis_event) == -1) {
                perror("failed to add incoming socket to epoll");
                exit(1);
            }
            strncpy(clients_name[CLIENTS], msg.name, MAX_NAME_LEN - 1);
            clients[CLIENTS++] = infd;
            fprintf(stderr, "added %s at no %d", msg.name, CLIENTS - 1);

        }


    }
}


void *network_job(void *arg) {
    create_internet_socket();
    create_local_socket();
    make_sockets_non_blocking();
    listen_sockets();
    init_epoll();
    work();


}

void delegate(Task *task) {
    int worker;
    pthread_mutex_lock(&clients_array_mutex);
    if (CLIENTS == 0) {
        fprintf(stderr, "no clients!\n");
        pthread_mutex_unlock(&clients_array_mutex);
        return;
    }
    worker = clients[rand() % CLIENTS];
    pthread_mutex_unlock(&clients_array_mutex);
    Message msg;
    create_msg(&msg, task, TASK);
    if (-1 == send(worker, &msg, sizeof(msg), 0))
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

