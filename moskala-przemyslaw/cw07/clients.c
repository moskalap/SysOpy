//
// Created by przemek on 02.05.17.
//

#include "clients.h"

int shmID;
int semID;
key_t KEY;
struct sembuf sops[3];

int main(int argc, int *argv[]) {
    char *path = getenv(env);
    key_t key = ftok(path, PROJECT_ID);
    KEY = key;
    Waiting_Room *wr = enter_waiting_room();
    /*if (barber_sleep(wr)) {
        wake_up_him();
    } else {
        if (free_seats(wr)) {
            queue_self();
        } else {
            printf("There is no free seats :(\n";
            exit(-1);

        }
    }
*/
    get_semaphore();
    turn_semaphore_on(ACCESS_SHM);
    add_customer(wr, getpid());
    turn_semaphore_off(ACCESS_SHM);

    turn_semaphore_on(ACCESS_SHM);
    add_customer(wr, getpid());
    turn_semaphore_off(ACCESS_SHM);

    turn_semaphore_on(ACCESS_SHM);
    add_customer(wr, getpid());
    turn_semaphore_off(ACCESS_SHM);


    return 0;
}

Waiting_Room *enter_waiting_room() {
    Waiting_Room *wr;
    shmID = shmget(KEY, sizeof(Waiting_Room), IPC_CREAT | 0666);
    if (shmID == -1) {
        perror("Error while getting shared memory\n");
        exit(-1);
    }
    void *ptr = shmat(shmID, NULL, 0);
    if (ptr == (void *) (-1)) {
        perror("Error while accessing a shared memory\n");
        exit(-1);
    }
    wr = (Waiting_Room *) ptr;
    return wr;
}

void get_semaphore() {

    if (-1 == (semID = semget(KEY, 0, 0666))) {
        perror("init erro1r");
        exit(-1);
    }
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