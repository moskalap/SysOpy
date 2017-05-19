//
// Created by przemek on 02.05.17.
//



#include "clients.h"

int shmID;
int semID;
key_t KEY;


sem_t *ACCESS_SHM;
sem_t *FREE_SEATS;
sem_t *CUSTOMERS_IN_QUEUE;
sem_t *CHAIR;
sem_t *PAYMENT;

int SHAVES = 9;
int SHAVES_GOT = 1;

void shave_hander(int n) {
    SHAVES_GOT++;

}

int CLIENTS;

int main(int argc, int *argv[]) {
    signal(SIGUSR1, shave_hander);
    CLIENTS = atoi(argv[1]);
    SHAVES = atoi(argv[2]);
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

    dec_sem(ACCESS_SHM);
    if (-1 == sem_trywait(FREE_SEATS)) {

        inc_sem(ACCESS_SHM);
        return 0;

    }

    if (w->last == 0) {
        printf("%d Wokes up barber\n", getpid());
    } else {
        printf("%d Wait in queue\n", getpid());
    }

    add_customer(w, getpid());
    inc_sem(ACCESS_SHM);
    inc_sem(CUSTOMERS_IN_QUEUE);

    return 1;
}

void wait_for_shave(Waiting_Room *waiting_room) {
    pid_t myPID = getpid();
    pid_t fpid = 0;

    while (myPID != fpid) {
        dec_sem(ACCESS_SHM);
        fpid = waiting_room->queue[0];
        inc_sem(ACCESS_SHM);

    }
    inc_sem(CHAIR);
    printf("%dJust have been shaved\n", getpid());
    dec_sem(PAYMENT);


}


Waiting_Room *enter_waiting_room() {
    Waiting_Room *wr;
    shmID = shm_open(shmpth, O_RDWR, 0666);
    if (shmID == -1) {
        perror("Error while getting shared memory\n");
        exit(-1);
    }
    if (ftruncate(shmID, sizeof(Waiting_Room)) == -1) {
        perror("error while truncato");
        exit(-1);
    }

    void *ptr = mmap(NULL, sizeof(Waiting_Room), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    if (ptr == (void *) (-1)) {
        perror("Error while accesing a shared memory\n");
        exit(-1);
    }
    wr = (Waiting_Room *) ptr;

    return wr;
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

void get_semaphore() {

    FREE_SEATS = sem_open(freeseats, O_RDWR);
    if (FREE_SEATS == SEM_FAILED) {
        perror("init erro1r");
        exit(-1);
    }

    PAYMENT = sem_open(payment, O_RDWR);
    if (PAYMENT == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }

    CHAIR = sem_open(chair, O_RDWR);
    if (CHAIR == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }

    ACCESS_SHM = sem_open(accshm, O_RDWR);
    if (ACCESS_SHM == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }

    CUSTOMERS_IN_QUEUE = sem_open(customersinqueue, O_RDWR);
    if (CUSTOMERS_IN_QUEUE == SEM_FAILED) {
        perror("init error");
        exit(-1);
    }


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
