//
// Created by przemek on 02.05.17.
//


#include <signal.h>
#include <bits/time.h>
#include <time.h>

#include "barber.h"
#include <sys/time.h>
#include "sys/mman.h"
#include "sys/stat.h"
#include "fcntl.h"
#include <semaphore.h>


sem_t *ACCESS_SHM;
sem_t *FREE_SEATS;
sem_t *CUSTOMERS_IN_QUEUE;
sem_t *CHAIR;
sem_t *PAYMENT;
int STS = 8;
int shmID;
int bropen = 1;

void endfun(int n) {
    shmctl(shmID, IPC_RMID, NULL);
    bropen = 0;
}

int main(int argc, char *argv[]) {

    char *path = getenv(env);

    signal(SIGINT, endfun);

    if (argc != 2) {
        printf("usage: barber amount_of_seats\n");
        exit(0);
    } else {
        STS = atoi(argv[1]);
    }

    make_semaphores();


    Waiting_Room *wr = open_barber_shop(5);


    while (bropen) {
        are_beards_to_shave(wr);

        shave_client(wr);


    }


}

void are_beards_to_shave(Waiting_Room *waiting_room) {
    if (waiting_room->last == 0)
        printf("%ldBarber goes to sleep.\n", time_stamp());
    dec_sem(CUSTOMERS_IN_QUEUE);
    printf("%ldBarber woke up\n", time_stamp());


}

void shave_client(Waiting_Room *waiting_room) {
    dec_sem(ACCESS_SHM);
    pid_t to_shave = get_first_in_queue(waiting_room);
    inc_sem(FREE_SEATS);
    inc_sem(ACCESS_SHM);
    dec_sem(CHAIR);
    printf("%ld started shaving%d!\n", time_stamp(), to_shave);
    kill(to_shave, SIGUSR1);

    inc_sem(PAYMENT);
    printf("%ld end shaving%d!\n", time_stamp(), to_shave);

}


void dec_sem(sem_t *name) {

    if (-1 == sem_wait(name)) {
        perror("using semaphore dec");
        exit(-1);
    }

}

void inc_sem(sem_t *name) {
    if (-1 == sem_post(name)) {
        perror("using semaphore inc");
        exit(-1);
    }
}

void make_semaphores() {
    FREE_SEATS = sem_open(freeseats, O_CREAT | O_EXCL | O_RDWR, 0666, STS);
    if (FREE_SEATS == SEM_FAILED) {
        perror("init erro1r");
        exit(-1);
    }

    PAYMENT = sem_open(payment, O_CREAT | O_EXCL | O_RDWR, 0666, 0);
    if (PAYMENT == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }

    CHAIR = sem_open(chair, O_CREAT | O_EXCL | O_RDWR, 0666, 0);
    if (CHAIR == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }

    ACCESS_SHM = sem_open(accshm, O_CREAT | O_EXCL | O_RDWR, 0666, 1);
    if (ACCESS_SHM == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }

    CUSTOMERS_IN_QUEUE = sem_open(customersinqueue, O_CREAT | O_EXCL | O_RDWR, 0666, 0);
    if (CUSTOMERS_IN_QUEUE == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }


}

Waiting_Room *open_barber_shop(int seats) {
    Waiting_Room *wr;
    shmID = shm_open(shmpth, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shmID == -1) {
        perror("Error while getting shared memory\n");
        exit(-1);
    }
    if (ftruncate(shmID, sizeof(Waiting_Room)) == -1) {
        perror("error while trnct");
        exit(-1);
    }

    void *ptr = mmap(NULL, sizeof(Waiting_Room), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    if (ptr == (void *) (-1)) {
        perror("Error while accesing a shared memory\n");
        exit(-1);
    }
    wr = (Waiting_Room *) ptr;
    wr->seats = seats;
    return wr;


}

struct timeval GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

long time_stamp() {
    struct timeval tv = GetTimeStamp();
    long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
    return time_in_micros;
}

