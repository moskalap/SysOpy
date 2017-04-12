//
// Created by przemek on 05.04.17.
//
int actual = 0;

#include "interpreter.h"

void sig_handler(int n) {
    //Just a handler for interupt pause();
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
void display_executable(Executable *e, int fd[]) {
    fprintf(stderr, "Program name: %s.\n", e->program_name);
    printf("\n PPID %d, PID %d Writing to%d from %d Program name: %s.\n", getppid(), getpid(), fd[1], fd[0],
           e->program_name);
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
    if (execvp(ex->args[0], ex->args) == -1)
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
        e->input = NULL;
        e->output = NULL;
        pipe->last = e;
    } else {
        pipe->last->output = e;
        e->input = pipe->last;
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


void execute_all(Pipe *p) {
    Executable *task = malloc(sizeof(Executable));
    task->input = p->last;
    int n = p->n;
    int fd[2];
    pid_t pid;
    pipe:
    pipe(fd);

    create:
    n--;
    if (task)
        task = task->input;
    pid = fork();
    if (pid == 0) goto child;
    else goto parent;

    parent:
    if (getpid() == START_PROCESS_PID) goto interpreter;
    dup2(fd[0], STDIN_FILENO);
    close(fd[1]);
    pause(); // Wait for a SIGALARM from child ( permission to start own task )
    wait(NULL);
    kill(getppid(), SIGALRM);  // send permission to parent
    display_executable(task, fd);
    exec_token(task);
    exit(0);


    child:
    if (getppid() != START_PROCESS_PID) dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);

    if (n >= 1) goto pipe;
    else kill(getppid(), SIGALRM);


    interpreter:
    wait(NULL);
    exit(0);

}


int main(int argc, char *argv[]) {
    signal(SIGALRM, sig_handler);
    signal(SIGCHLD, sig_handler);
    START_PROCESS_PID = getpid();

    char buf[] = "ls -la | grep Makefile | wc | wartownik";
    Pipe *p = build_from_args(buf);

    execute_all(p);

}
