//
// Created by przemek on 02.05.17.
//

#include "waiting_room.c"

#ifndef CW07Z1_BARBER_H
#define CW07Z1_BARBER_H

Waiting_Room *open_barber_shop(int seats);

void make_semaphores();

long time_stamp();

void are_beards_to_shave(Waiting_Room *waiting_room);

void shave_client(Waiting_Room *waiting_room);

void go_sleeping();
void turn_semaphore_on(unsigned short name);

void use_semaphore(unsigned short name, short val);

void turn_semaphore_off(unsigned short name);

#endif //CW07Z1_BARBER_H
