//
// Created by przemek on 21.05.17.
//

#include <time.h>
#include "reader_writer_posix.h"

int READERS = 0;
int WRITERS = 0;
int verbose = 0;
int N = 0;
int *T;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    parse_args(argc, argv);
    T = malloc(N * sizeof(int));


}

void do_writer_job(void *arg) {
    int mods;
    int pos;
    int val;
    while (1) {
        mods = rand() % N;
        for (int i = 0; i < mods; i++) {
            pos = rand() % N;
            val = rand() % N;
            T[pos] = val;
            if (verbose)
                printf("writer:\t T[%d]=%d\n", pos, val);
        }
    }

}

void do_reader_job(void *arg) {

    int mod = (int) arg;
    int found = 0;
    while (1) {
        found = 0;
        for (int i = 0; i < N; i++) {

            if (T[i] % mod == 0) {
                found++;
                if (verbose)
                    printf("reader\tT[%d] = %d\n", i, T[i]);
            }
        }
        printf("reader\tFound %d numbers", found);
    }

}

void parse_args(int argc, char *argv[]) {
    if (argc == 4) {
        READERS = atoi(argv[1]);
        WRITERS = atoi(argv[2]);
        N = atoi(argv[3]);
    } else {
        printf("usage: ./reader_writer_posix READERS WRITERS N");
        exit(0);
    }
}

