#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <wait.h>

const char * ERR_FILE="\nError! The file doesn't exist or you haven't permission to read!\n";
const char * ERR_NOARG="\nError! Program argument not specified!\n";
enum Tasks {variable, process};


enum Tasks get_category(char* line){
    if(line[0]=='#') return variable;
    return process;
}
void execute_program(char* program_name, char* args[], int pargc){
    pid_t pid=fork();
    if(pid==0){
        execv(program_name,args);
        exit(0);
    }
    else{
        waitpid(pid,0,0);
    }
}

void declare_from_line(char * line){
    char* variable_name;
    char* variable_value;
    strtok(line,"#");
    variable_name=strtok(line," ");
    variable_value=strtok(line," ");
    setenv(variable_name,variable_value,1);
}
void delete_variable(char * line){
    strtok(line,"#");
    unsetenv(line);
}
int is_declaration(char * line){
    strtok(line, " ");
    if (strtok(line, " ")==NULL) return 0;
    return 1;
}
int count_args(char * line){
    int i;
    for(i=0;strtok(line, " ")!=NULL;i++);
    return i;

}
char* get_program_name(char* line){
    return strtok(line, " ");
}
char** get_args(char* line){
    char ** args=malloc(sizeof(char*)*count_args(line));
}
void execute_tasks(FILE* file){
    char * line;
    size_t len=0;
    while(getline(&line,&len, file)!=-1){
        switch(get_category(line)){
            case variable:
                if(is_declaration(line) == 1) declare_from_line(line);
                else delete_variable(line);
                break;
            case process:
                execute_program(get_program_name(line), get_args(line), count_args(line));
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

int main (int argc, char *argv[]){
    FILE * file=parse_args(argc, argv);
    if(file) execute_tasks(file);
    else printf(ERR_FILE);
}