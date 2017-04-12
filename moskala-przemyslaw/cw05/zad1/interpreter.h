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

typedef struct Executable{

    char * program_name;
    char ** args;
    struct Executable * output;


}Executable;


typedef struct Pipe{
    Executable * first;
    Executable * last;
}Pipe;

Executable * create_executable(char* buff);
Pipe * create_pipe();
void add_executable_to_pipes(Executable * executable, Pipe * pipe);


#endif //CW04C03_TEST_INTERPRETER_H
