//
// Created by przemek on 02.05.17.
//


#include <signal.h>
#include <bits/time.h>
#include <time.h>
#include <sys/time.h>
#include "barber.h"

int STS;
int shmID;
int semID;
int bropen = 1;
key_t KEY;

void endfun(int n) {
    shmctl(shmID, IPC_RMID, NULL);
    bropen = 0;
}

struct sembuf sops[3];

int main(int argc, char *argv[]) {

    char *path = getenv(env);
    key_t key = ftok(path, PROJECT_ID);
    KEY = key;
    signal(SIGINT, endfun);
    shmctl(shmID, IPC_RMID, NULL);
    if (argc != 2) {
        printf("usage: barber [amount_of_seats]\n");
        exit(0);
    } else
        STS = atoi(argv[1]);
    make_semaphores();
    int sh = semID;

    Waiting_Room *wr = open_barber_shop(STS);


    while (bropen) {
        are_beards_to_shave(wr);

        shave_client(wr);


    }

    semctl(semID, 0, IPC_RMID);
    semctl(semID, 1, IPC_RMID);
    semctl(semID, 2, IPC_RMID);
    semctl(semID, 3, IPC_RMID);
    semctl(semID, 4, IPC_RMID);
}

void are_beards_to_shave(Waiting_Room *waiting_room) {
    if (waiting_room->last == 0)
        printf("%ldBarber goes to sleep.\n", time_stamp());
    use_semaphore(CUSTOMERS_IN_QUEUE, -1); //sleps until there a re customers
    printf("%ldBarber woke up\n", time_stamp());


}

void shave_client(Waiting_Room *waiting_room) {
    use_semaphore(ACCESS_SHM, -1);
    pid_t to_shave = get_first_in_queue(waiting_room);
    use_semaphore(FREE_SEATS, 1);
    use_semaphore(ACCESS_SHM, 1);

    use_semaphore(CHAIR, -1);
    printf("%ld started shaving%d!\n", time_stamp(), to_shave);
    kill(to_shave, SIGUSR1);


    use_semaphore(PAYMENT, 1);
    printf("%ld end shaving%d!\n", time_stamp(), to_shave);


}

void go_sleeping() {
    sops[0].sem_op = -1;
    sops[0].sem_num = CUSTOMERS_IN_QUEUE;
    sops[0].sem_flg = 0;
    if (-1 == semop(semID, sops, 1)) //Weak up the Barber
        perror("waiting for ready cust");

}

void use_semaphore(unsigned short name, short val) {
    sops[0].sem_op = val;
    sops[0].sem_num = name;
    sops[0].sem_flg = 0;
    if (-1 == semop(semID, sops, 1)) //Weak up the Barber
    {
        perror("using semaphore");
        exit(-1);
    }

}

void make_semaphores() {

    semID = semget(KEY, 5, IPC_CREAT | 0666);
    if (-1 == semctl(semID, FREE_SEATS, SETVAL, STS)) {
        perror("init erro1r");
        exit(-1);
    }

    if (-1 == semctl(semID, PAYMENT, SETVAL, 0)) {
        perror("pymnt erro1r");
        exit(-1);
    }
    if (-1 == semctl(semID, CHAIR, SETVAL, 0)) {
        perror("CHAIR erro1r");
        exit(-1);
    }
    if (-1 == semctl(semID, ACCESS_SHM, SETVAL, 1)) {
        perror("init error");
        exit(-1);
    }
    if (-1 == semctl(semID, CUSTOMERS_IN_QUEUE, SETVAL, 0)) {
        perror("init error");
        exit(-1);
    }

}

Waiting_Room *open_barber_shop(int seats) {
    Waiting_Room *wr;
    shmID = shmget(KEY, sizeof(Waiting_Room), IPC_CREAT | 0666);
    if (shmID == -1) {
        perror("Error while getting shared memory\n");
        exit(-1);
    }
    void *ptr = shmat(shmID, NULL, 0);
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

long int time_stamp() {
    struct timeval tv = GetTimeStamp();
    long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
    return time_in_micros;
}

void turn_semaphore_on(unsigned short name) {
    sops[0].sem_num = name;
    sops[1].sem_num = name;
    sops[0].sem_op = 0;
    sops[0].sem_flg = 0;
    sops[1].sem_flg = 0;
    sops[1].sem_op = 1;

    if (-1 == semop(semID, sops, 2))
        perror("turn sem on");
}

void turn_semaphore_off(unsigned short name) {
    sops[0].sem_num = name;
    sops[0].sem_op = -1;
    sops[0].sem_flg = -1;
    if (-1 == semop(semID, sops, 1))
        perror("turn sem off");

}