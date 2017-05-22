//
// Created by przemek on 21.05.17.
//

#ifndef READER_WRITER_MUTEX_READER_WRITER_MUTEX_H
#define READER_WRITER_MUTEX_READER_WRITER_MUTEX_H

#include "stdlib.h"
#include "stdio.h"
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <zconf.h>

void parse_args(int argc, char *argv[]);

void do_writer_job(void *arg);

void do_reader_job(void *arg);

void make_semaphore();

void destroy_semaphore(sem_t *a, char *b);

#endif //READER_WRITER_MUTEX_READER_WRITER_MUTEX_H
