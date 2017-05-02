//
// Created by przemek on 02.05.17.
//

#include "waiting_room.c"

#ifndef CW07Z1_BARBER_H
#define CW07Z1_BARBER_H

Waiting_Room *open_barber_shop(int seats);

void make_semaphores();

void turn_semaphore_on(unsigned short name);

void turn_semaphore_off(unsigned short name);

#endif //CW07Z1_BARBER_H
