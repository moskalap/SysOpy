//
// Created by przemek on 05.04.17.
//

#ifndef CW04C03_TEST_INTERPRETER_H
#define CW04C03_TEST_INTERPRETER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <wait.h>
#include "string.h"
const int MAX_ARGS=20;
const int MAX_TOKENS=30;
pid_t START_PROCESS_PID;


typedef struct Executable{

    char * program_name;
    char ** args;
    struct Executable * output;
    struct Executable * input;


}Executable;


typedef struct Pipe{
    Executable * first;
    Executable * last;
    int n;
}Pipe;

Executable * create_executable(char* buff);
Pipe * create_pipe();
void add_executable_to_pipe(Pipe * pipe, Executable * executable);


#endif //CW04C03_TEST_INTERPRETER_H
