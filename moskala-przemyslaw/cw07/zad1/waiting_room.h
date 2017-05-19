//
// Created by przemek on 02.05.17.
//
#include <zconf.h>

#include <stdio.h>
#include "sys/shm.h"
#include "sys/types.h"
#include "sys/ipc.h"

#ifndef CW07Z1_WAITING_ROOM_H
#define CW07Z1_WAITING_ROOM_H


#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define ACCESS_SHM 1
#define FREE_SEATS 0
#define CUSTOMERS_IN_QUEUE 2
#define CHAIR 3
#define PAYMENT 4


#define PROJECT_ID 23
const char env[] = "HOME";

typedef struct Waiting_Room {
    pid_t queue[100];
    int first;
    int last;
    int seats;
    int customers;


} Waiting_Room;

Waiting_Room *create_wr(int seats);

int add_customer(Waiting_Room *waiting_room, pid_t customer);

pid_t get_first_in_queue(Waiting_Room *wr);

#endif //CW07Z1_WAITING_ROOM_H
