
// Created by przemek on 23.03.17.
//
//fork() - nowy proces - > exec ,, -> w macierzzystym czekamy na wait
//z2: setrlimit -
const char * ERR_FILE="\nError! The file doesn't exist or you haven't permission to read!\n";
const char * ERR_NOARG="\nError! Program argumment not specified!\n";
enum Tasks {variable, process};

void execute_tasks(FILE* file){
    getline();
    line=get_next_line(file);

}

FILE * parse_args(int argc, char * argv[]){
    if(argc<2){
        printf(ERR_NOARG);
        return NULL;
    }
    return fopen(argv[1],"r");



}

int main (int argc, char *argv[]){
    int file=parse_args(argc, argv);
    if(file) execute_tasks(file);
    else printf(ERR_FILE);
}
