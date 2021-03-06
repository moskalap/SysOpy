#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <wait.h>
#include <sys/resource.h>
#include <sys/wait.h>
const char * ERR_FILE="\nError! The file doesn't exist or you haven't permission to read!\n";
const char * ERR_NOARG="\nError! Program argument not specified!\n";
const int MAX_ARGS =3;
enum Tasks {variable, process};
enum Tasks get_category(char* line){
    if(line[0]=='#') return variable;
    return process;
}
void execute_program(char* args[MAX_ARGS]){
    int status;
    struct rusage r;
    pid_t pid=fork();
    if(pid<0){
        printf("Error: can not create process for:%s", args);
        exit(EXIT_FAILURE);
    }else {
        if (pid==0){

            execvp(args[0], args) == -1 ? exit(EXIT_FAILURE):exit(EXIT_SUCCESS);
        }
        else{
            pid = wait3(&status, 0, &r);
            if(WEXITSTATUS(status) != 0){
                fprintf(stderr, "Error while executing command %s", args[0]);
                exit(WEXITSTATUS(status));
            }
        }
    }
}

void declare_from_line(char * line[]){
    char* name = line[0];
    char* val=line[1];
    name++;
    setenv(name,val,1);

}
void delete_variable(char * line){
    ++line;
    unsetenv(line);
}
int is_declaration(char * line, char * arr[]){
    char *p;
    int i = 0;
    p=strtok(line," ");
    int f=1;
    while (p != NULL){
        f++;
        arr[i++] = p;
        p = strtok (NULL, " ");

    }
    if(f==2) return 0;
    return 1;
}
void execute_tasks(FILE* file){
    char * line;
    char * a[MAX_ARGS];
    char* variables[2];
    size_t len=0;
    while(getline(&line,&len, file)!=-1){
        line[strcspn(line, "\n")] = 0;
        switch(get_category(line)){
            case variable:
                if(is_declaration(line, variables) == 1) declare_from_line(variables);
                else delete_variable(variables[0]);
                break;
            case process:
                get_args(line, a);;
                execute_program(a);
                break;
            default:
                break;
        }
    }

}

FILE * parse_args(int argc, char * argv[]){
    if(argc<2){
        printf(ERR_NOARG);
        return NULL;
    }
    return fopen(argv[1],"r");
}
void get_args(char line[], char * array[]){
    char *p;
    int i = 0;
    p=strtok(line," ");
    int o=0;
    for(o=0; o<MAX_ARGS; o++){
        array[o]=NULL;
    }

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, " ");
    }
}

int main (int argc, char *argv[]){

    FILE * file=parse_args(argc, argv);
    if(file) execute_tasks(file);
    else printf(ERR_FILE);

}