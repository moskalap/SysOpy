//
// Created by przemek on 02.05.17.
//
#include "waiting_room.h"
#include "waiting_room.c"


#ifndef CW07Z1_CLIENTS_H
#define CW07Z1_CLIENTS_H

Waiting_Room *enter_waiting_room();

void get_semaphore();

void turn_semaphore_on(unsigned short name);

void turn_semaphore_off(unsigned short name);

#endif //CW07Z1_CLIENTS_H
