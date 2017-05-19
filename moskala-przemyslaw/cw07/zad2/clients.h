//
// Created by przemek on 10.05.17.
//

#ifndef CW07Z2B_CLIENTS_H
#define CW07Z2B_CLIENTS_H

#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "waiting_room.c"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "waiting_room.h"

int can_be_shaved(Waiting_Room *w);

void wait_for_shave(Waiting_Room *waiting_room);

Waiting_Room *enter_waiting_room();

void dec_sem(sem_t *name);

void inc_sem(sem_t *name);

void get_semaphore();

struct timeval GetTimeStamp();

long time_stamp();

#endif //CW07Z2B_CLIENTS_H
