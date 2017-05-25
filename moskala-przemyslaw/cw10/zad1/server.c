//
// Created by przemek on 25.05.17.
//
#include "server.h"
#include<arpa/inet.h> //inet_addr

int socket_desc, client_socket, c;


void initialize_server();

int main(int argc, char *argv[]) {
    initialize_server();


    return 0;
}

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