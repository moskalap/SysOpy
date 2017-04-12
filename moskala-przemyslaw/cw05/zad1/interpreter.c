//
// Created by przemek on 05.04.17.
//

#include "interpreter.h"
void sig_handler(int n){
    return;
}
/**
 * Creates an arguments as array of strings.
 * @param buff buffer of chars betwen '|'
 * @return array of args
 */
char **create_array(char *buff) {
    char **parmList = malloc(sizeof(char *) * MAX_ARGS);
    int i = 0;
    char *p = strtok(buff, " ");
    while (p != NULL) {
        parmList[i] = malloc(sizeof(char) * strlen(p));
        strcpy(parmList[i], p);
        i++;
        p = strtok(NULL, " ");
    }
    parmList[i] = NULL;
    free(p);
    return parmList;

}

/**
 * creates a struct Pipe w
 * @param token
 * @return
 */
Executable *create_executable(char *token) {

    Executable *executable = malloc(sizeof(Executable));
    executable->args = malloc(MAX_ARGS * sizeof(char *));
    executable->args = create_array(token);
    executable->program_name = executable->args[0];

}

/**
 * displays content of executable struct
 * @param e
 */
void display_executable(Executable *e) {
    printf("Program name: %s.\n", e->program_name);
    int i = 0;
    for (i; i < MAX_ARGS; i++) {
        printf("Arg no %d: %s", i, e->args[i]);

    }

}

/**
 * executes a single token
 * @param pipe
 */
void exec_token(Executable *ex) {
    if (execv(ex->args[0], ex->args) == -1)
        fprintf(stderr, "error while executing %s", ex->program_name);
}

/**
 * Creates a structure which contains executables
 * @return pointer to Pipe
 */
Pipe *create_pipe() {
    Pipe *pipe = malloc(sizeof(Pipe));
    pipe->first = NULL;
    pipe->last = NULL;
    pipe->n = 0;
    return pipe;
}

void add_executable_to_pipe(Pipe *pipe, Executable *e) {
    if (pipe->n == 0) {
        pipe->first = e;
        pipe->last = e;
    } else {
        pipe->last->output = e;
        pipe->last = e;
    }
    pipe->n++;

}

void execute_without_redir(Pipe *pipe) {
    Executable *tmp = pipe->first;

    for (int i = 0; i < pipe->n; i++) {
        pid_t id = fork();
        if (id == 0) {
            exec_token(tmp);
            exit(0);
        } else {
            waitpid(id, NULL, NULL);
        }
        tmp = tmp->output;
    }
}

void execute_all(Pipe * tasks){
    pid_t pid;
    int fd[2];

    becoming_parent:
    pipe(fd);

        pid=fork();
    if(pid==0){
        dup2(fd[0],STDIN_FILENO);
        pause();
        tasks->first=tasks->first->output;
        if (tasks && tasks->first) goto becoming_parent;

    }
    else{
        dup2(fd[1],STDOUT_FILENO);
        exec_token(tasks->first);
        kill(pid, SIGUSR1); // permission to child for becoming parent
        waitpid(pid, NULL, NULL);
        if(getpid()!=START_PROCESS_PID) exit(0);
    }




}

Pipe *build_from_args(char *args) {
    Pipe *pipe = create_pipe();
    char *tmp;
    char **tokens_list = malloc(sizeof(char *) * MAX_TOKENS);
    int i = 0;
    char *p = strtok(args, "|");
    while (p != NULL) {

        tokens_list[i] = malloc(strlen(p) * sizeof(char));
        strcpy(tokens_list[i], p);
        p = strtok(NULL, "|");
        i++;

    }

    for (int j = 0; j < i; j++) {
        add_executable_to_pipe(pipe, create_executable(tokens_list[j]));
    }


    return pipe;
}

int main(int argc, char *argv[]) {
    signal(SIGUSR1, sig_handler);
    START_PROCESS_PID = getpid();
    char buf[] = "/bin/ls -la -r|/bin/wc";
    Pipe *p = build_from_args(buf);
    //execute_without_redir(p);
    execute_all(p);

}
