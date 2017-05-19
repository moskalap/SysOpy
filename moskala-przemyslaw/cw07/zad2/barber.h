//
// Created by przemek on 03.05.17.
//

#ifndef CW07Z1C_BARBER_H
#define CW07Z1C_BARBER_H

#include <semaphore.h>
#include "waiting_room.h"
#include "waiting_room.c"

void are_beards_to_shave(Waiting_Room *waiting_room);

void shave_client(Waiting_Room *waiting_room);

void dec_sem(sem_t *name);

void inc_sem(sem_t *name);

void make_semaphores();

long time_stamp();

Waiting_Room *open_barber_shop(int seats);

#endif //CW07Z1C_BARBER_H
