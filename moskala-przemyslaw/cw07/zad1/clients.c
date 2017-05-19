//
// Created by przemek on 02.05.17.
//

#include <bits/signum.h>
#include <signal.h>
#include "clients.h"

int shmID;
int semID;
key_t KEY;
struct sembuf sops[3];
int SHAVES;
int SHAVES_GOT = 1;
int CLIENTS;

void shave_hander(int n) {
    SHAVES_GOT++;

}


int main(int argc, char *argv[]) {
    CLIENTS = atoi(argv[1]);
    SHAVES = atoi(argv[2]);

    signal(SIGUSR1, shave_hander);
    char *path = getenv(env);
    key_t key = ftok(path, PROJECT_ID);
    KEY = key;
    Waiting_Room *wr = enter_waiting_room();
    get_semaphore();
    for (int p = 0; p < CLIENTS; p++)
        if (fork() == 0)
            while (SHAVES_GOT != SHAVES) {


                if (can_be_shaved(wr)) {
                    wait_for_shave(wr);
                } else {
                    printf("There is no placce %d left without cut\n", getpid());
                }


            }
    return 0;
}

int can_be_shaved(Waiting_Room *w) {
    //turn_semaphore_on(ACCESS_SHM);
    use_semaphore(ACCESS_SHM, -1);
    sops[0].sem_num = FREE_SEATS;
    sops[0].sem_op = -1;
    sops[0].sem_flg = IPC_NOWAIT;
    if (-1 == semop(semID, sops, 1)) {
        //  turn_semaphore_off(ACCESS_SHM);
        sops[0].sem_flg = 0;
        use_semaphore(ACCESS_SHM, 1);
        return 0;

    }
    sops[0].sem_flg = 0;
    if (w->last == 0) {
        printf("%d Wokes up barbre\n", getpid());
    } else {
        printf("%d Wait in queue\n", getpid());
    }

    add_customer(w, getpid());
    use_semaphore(ACCESS_SHM, 1);
    use_semaphore(CUSTOMERS_IN_QUEUE, 1);
    //turn_semaphore_off(ACCESS_SHM);
    return 1;
}

void wait_for_shave(Waiting_Room *waiting_room) {
    pid_t myPID = getpid();
    pid_t fpid = 0;

    while (myPID != fpid) {
        use_semaphore(ACCESS_SHM, -1);
        fpid = waiting_room->queue[0];

        use_semaphore(ACCESS_SHM, 1);

    }
    use_semaphore(CHAIR, 1);
    printf("%dJust been shabed\n", getpid());
    use_semaphore(PAYMENT, -1);


}

int try_to_take_a_seat(Waiting_Room *waiting_room) {
    turn_semaphore_on(ACCESS_SHM);
    if (waiting_room->last == waiting_room->seats) {
        turn_semaphore_off(ACCESS_SHM);
        return 0;
    }
    if (waiting_room->last == waiting_room->first) {
        add_customer(waiting_room, getpid());
        turn_semaphore_off(ACCESS_SHM);
        sops[0].sem_op = 1;
        sops[0].sem_num = CUSTOMERS_IN_QUEUE;
        if (-1 == semop(semID, sops, 1)) //Weak up the Barber
            perror("waiting for ready cust");
        return 1;
    }
    add_customer(waiting_room, getpid());
    turn_semaphore_off(ACCESS_SHM);


    turn_semaphore_off(ACCESS_SHM);
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

void use_semaphore(unsigned short name, short val) {
    sops[0].sem_op = val;
    sops[0].sem_num = name;
    sops[0].sem_flg = 0;
    if (-1 == semop(semID, sops, 1)) //Weak up the Barber
        perror("using semaphore");

}