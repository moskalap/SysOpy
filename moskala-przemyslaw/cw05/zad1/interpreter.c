//
// Created by przemek on 05.04.17.
//

#include "interpreter.h"

/**
 * Creates an arguments as array of strings.
 * @param buff buffer of chars betwen '|'
 * @return array of args
 */
char** create_array(char * buff){
    char ** parmList =malloc(sizeof(char*)*MAX_ARGS);
    int i = 0;
    char *p = strtok (buff, " ");
    while (p != NULL) {
        parmList[i]=malloc(sizeof(char)*strlen(p));
        strcpy(parmList[i], p);
        i++;

        p = strtok (NULL, " ");
    }
    free(p);
    return parmList;

}
/**
 * creates a struct Pipe w
 * @param token
 * @return
 */
Executable * create_executable(char * token){

    Executable * executable = malloc(sizeof(Executable));
    executable->args=malloc(MAX_ARGS*sizeof(char*));
    executable->args = create_array(token);
    executable->program_name=executable->args[0];

}

/**
 * displays content of executable struct
 * @param e
 */
void display_executable(Executable * e){
    printf("Program name: %s.\n", e->program_name);
    int i=0;
    for ( i ; i<MAX_ARGS ; i++){
        printf("Arg no %d: %s",i, e->args[i]);

    }

}
/**
 * executes a single token
 * @param pipe
 */
void exec_token(Executable * ex){
    execv(ex->program_name, ex->args);
}
/**
 * Creates a structure which contains executables
 * @return pointer to Pipe
 */
Pipe * create_pipe(){
    Pipe * pipe = malloc(sizeof(Pipe));
    pipe->first=NULL;
    pipe->last=NULL;
    return pipe;
}


int main(int argc, char * argv[]){
    char  buf[] = "/bin/ls -l -R -a";
    exec_token(create_executable(buf));


}
