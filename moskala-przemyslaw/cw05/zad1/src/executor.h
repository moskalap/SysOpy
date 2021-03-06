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




#endif //CW04C03_TEST_INTERPRETER_H
