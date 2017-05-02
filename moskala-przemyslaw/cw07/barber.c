//
// Created by przemek on 02.05.17.
//


#include <signal.h>
#include "barber.h"

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
        printf("usage: barber amount_of_seats\n");
        exit(0);
    }

    make_semaphores();


    Waiting_Room *wr = open_barber_shop(5);

/*
turn_semaphore_on(ACCESS_SHM);

    add_customer(wr,getpid());
    add_customer(wr,getpid());
turn_semaphore_off(ACCESS_SHM);
    add_customer(wr,getpid());
    turn_semaphore_on(ACCESS_SHM);
    turn_semaphore_off(ACCESS_SHM);



    shmctl(shmID, IPC_RMID, NULL);
    /*open_barber_shop();
  */
    while (bropen) {
        while (are_beards_to_shave()) {
            shave_client();
        }
        go_sleeping();
    }
    close_barber_shop();
    * /


}

are_beards_to_shave()

        void make_semaphores(){

    semID = semget(KEY, 2, IPC_CREAT | 0666);
    if (-1 == semctl(semID, BARBER_SLEEP, SETVAL, 0)) {
        perror("init erro1r");
        exit(-1);
    }
    if (-1 == semctl(semID, ACCESS_SHM, SETVAL, 0)) {
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
    return wr;


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