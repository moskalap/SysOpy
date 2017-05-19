//
// Created by przemek on 02.05.17.
//

#include <stdlib.h>
#include "waiting_room.h"


Waiting_Room *create_wr(int seats) {
    Waiting_Room *wr = malloc(sizeof(Waiting_Room));
    wr->seats = seats;
    wr->last = 0;
    wr->first = 0;
    return wr;
}

int add_customer(Waiting_Room *waiting_room, pid_t customer) {
    int last = waiting_room->last;
    waiting_room->queue[last] = customer;
    waiting_room->customers++;
    waiting_room->last++;
    return last;
}

pid_t get_first_in_queue(Waiting_Room *wr) {
    if (wr->last == wr->first) return -1;
    pid_t c = wr->queue[0];
    int i;
    for (i = 1; i < wr->last; i++) {
        wr->queue[i - 1] = wr->queue[i];
    }
    wr->last--;
    wr->customers--;
    return c;

}

