//
// Created by przemek on 02.05.17.
//
#include "waiting_room.h"
#include "waiting_room.c"


#ifndef CW07Z1_CLIENTS_H
#define CW07Z1_CLIENTS_H

Waiting_Room *enter_waiting_room();

void get_semaphore();

int can_be_shaved(Waiting_Room *w);

int try_to_take_a_seat(Waiting_Room *waiting_room);

void leave();

void wait_for_shave(Waiting_Room *waiting_room);
void turn_semaphore_on(unsigned short name);

void turn_semaphore_off(unsigned short name);

#endif //CW07Z1_CLIENTS_H
