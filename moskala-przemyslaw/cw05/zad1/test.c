void test_exec_progs(){
    int     fd[2], nbytes;
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    readbuffer[80];

    pipe(fd);

    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if(childpid == 0) {
        close(fd[1]);


        dup2(fd[0],STDIN_FILENO);
        //execlp("grep lib", (char *)NULL);
        execlp("wc","wc", (char *)NULL);
        //execlp("grep ala","grep ala", (char *)NULL);

        // read(fd[0], readbuffer, sizeof(readbuffer));
        // printf("Received string: %s", readbuffer);

        exit(0);
    }
    else {

        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execlp("ls", "ls", "-l", "/", (char *)NULL);


    }



}
void test_structures(){
    Pipes * pipes= create_pipes_list();
    char ** strings = malloc(MAX_ARGS*sizeof(char*));
    //{"args","arg2"};


}
void test(){
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0){
        printf("In child");
        sleep(1);
        close(fd[1]);
        char * example;
        read(fd[0],example,10);
        printf("Recievied example %s", example);
        sleep(2);
    }else{
        if (pid >0){
            close(fd[0]);
            char * letter="ABCDEF";
            write(fd[1], letter, 10);
            wait(NULL);

        }if(pid<0){
            fprintf( stderr, "Error while forking.\n");
        }
    }
}

int test_string_sending(){
    int     fd[2], nbytes;
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    readbuffer[80];

    pipe(fd);

    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if(childpid == 0) {
        close(fd[0]);
        write(fd[1], string, (strlen(string)+1));
        exit(0);
    }
    else {

        close(fd[1]);
        read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s", readbuffer);
    }

    return(0);

}




















void execute_all(Pipe *tasks) {
    pid_t pid;
    int fd[2];

    becoming_parent:
    pipe(fd);
    if (tasks->first) {

        pid = fork();
        if (pid == 0) {

            pause();
            exec_token(tasks->first);
            printf("after");
            tasks->first = tasks->first->output;
            goto becoming_parent;

        } else {
            dup2(fd[1], STDOUT_FILENO);
            exec_token(tasks->first);
            kill(pid, SIGALRM); // permission to child for becoming parent
            waitpid(pid, NULL, NULL);
            //if (getpid() != START_PROCESS_PID) exit(0);
        }
    }
    while(1);

}











