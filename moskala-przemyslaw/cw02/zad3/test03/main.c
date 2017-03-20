#include <memory.h>
#include <fcntl.h>
#include <zconf.h>
#include "stdlib.h"
#include "stdio.h"
const int BFFR_L =256;
enum Options {
    lock_r, unlock_r, lock_w, unlock_w, reado, writeo, list, help, nothing, quit
};
enum Version{repeat, normal, nothingv};
enum Permission {writep, readp};
void display_man(){
    printf("\nUse:");
    printf("\n\t-lr\tfor locking read mode on byte,");
    printf("\n\t-ur\tfor unlocking read mode on byte,");
    printf("\n\t-lw\tfor locking write mode on byte,");
    printf("\n\t-uw\tfor locking write mode on byte,");
    printf("\n\t-r\tfor read a byte,");
    printf("\n\t-w\tfor write a byte,");
    printf("\n\t-l\tfor list locked files.\n");
}
int scan_byte_no(){
    char inp[BFFR_L];
    printf("Which byte do you want to lock?");
    fgets(inp,BFFR_L,stdin);
    return atoi(inp);
}
enum Version scan_version(){
    char inp;
    printf("Chose version (r for repating, s for single attempt)");
    inp=getchar();
    switch (inp){
        case 'r':
            return repeat;

        case 's':
            return normal;
        default:
            return nothing;

    }


}
enum Options decode(char input[BFFR_L]){
    if(strcmp(input,"-lr\n") ==0) return lock_r;
    if(strcmp(input,"-ur\n") ==0) return unlock_r;
    if(strcmp(input,"-lw\n") ==0) return lock_w;
    if(strcmp(input,"-uw\n") ==0) return unlock_w;
    if(strcmp(input,"-r\n") ==0) return reado;
    if(strcmp(input,"-w\n") ==0) return writeo;
    if(strcmp(input,"-l\n") ==0) return list;
    if(strcmp(input,"-help\n") ==0) return help;
    if(strcmp(input,"-quit\n") ==0) return quit;
    return nothing;
}
void lock(int file, int byte, enum Permission permission, enum Version v){
    struct flock *opt=malloc(sizeof(struct flock));
   switch (v){
       case repeat:
           switch(permission){
               case writep:
                    opt->l_type=F_WRLCK;
                    opt->l_whence=SEEK_SET;
                    opt->l_len=1;
                    opt->l_start=byte;
                    fcntl(file, F_SETLKW, opt);
                   break;
               case readp:
                   opt->l_type=F_RDLCK;
                   opt->l_whence=SEEK_SET;
                   opt->l_len=1;
                   opt->l_start=byte;
                   fcntl(file, F_SETLKW, opt);
                   break;
           }
           break;
       case normal:
           switch(permission){
               case writep:
                   opt->l_type=F_WRLCK;
                   opt->l_whence=SEEK_SET;
                   opt->l_len=1;
                   opt->l_start=byte;
                   fcntl(file, F_SETLK, opt);
                   break;
               case readp:
                   opt->l_type=F_RDLCK;
                   opt->l_whence=SEEK_SET;
                   opt->l_len=1;
                   opt->l_start=byte;
                   fcntl(file, F_SETLK, opt);
                   break;
           }
           break;

   }
}
void unlock(int file, int byte, enum Permission permission, enum Version v){
        struct flock *opt=malloc(sizeof(struct flock));
    switch(v){
        case repeat:
            switch(permission){
                case writep:
                    opt->l_type=F_UNLCK;
                    opt->l_whence=SEEK_SET;
                    opt->l_len=1;
                    opt->l_start=byte;
                    fcntl(file, F_SETLKW, opt);

                    break;
                case readp:
                    opt->l_type=F_UNLCK;
                    opt->l_whence=SEEK_SET;
                    opt->l_len=1;
                    opt->l_start=byte;
                    fcntl(file, F_SETLKW, opt);
                    break;
                default:
                    printf("perm err");

            }
            break;
        case normal:
            switch(permission){
                case writep:
                    opt->l_type=F_UNLCK;
                    opt->l_whence=SEEK_SET;
                    opt->l_len=1;
                    opt->l_start=byte;
                    fcntl(file, F_SETLK, opt);
                    break;
                case readp:
                    opt->l_type=F_UNLCK;
                    opt->l_whence=SEEK_SET;
                    opt->l_len=1;
                    opt->l_start=byte;
                    fcntl(file, F_SETLK, opt);
                    break;
                default:
                    printf("perm err");

            }
            break;
        default:
            printf("err v");
            break;

    }
}
void interact(char filename[BFFR_L]){

    char input[BFFR_L];
    int byte;
    enum Version v;


    int file;

    int run=1;
    while(run) {
        fgets(input, BFFR_L, stdin);
        printf(input);
        switch(decode(input)){
                case lock_r:
                file=open(filename, O_RDONLY);
                byte=scan_byte_no();
                v=scan_version();
                lock(file, byte, readp, v);

                break;
            case unlock_r:
                open(filename, O_RDONLY);
                byte=scan_byte_no();
                v=scan_version();
                unlock(file, byte, readp, v);
                break;
            case lock_w:
                open(filename, O_RDONLY);
                byte=scan_byte_no();
                v=scan_version();
                lock(file, byte, writep, v);
                break;
            case unlock_w:
                open(filename, O_RDONLY);
                byte=scan_byte_no();
                v=scan_version();
                unlock(file, byte, writep, v);
                break;
            case reado:

                open(filename, O_RDONLY);
                byte=scan_byte_no();
                //read_from(file, byte);

                break;
            case writeo:

               // open(file, O_RDONLY);
                byte=scan_byte_no();
                v=scan_version();
                //write_to(file, byte, new);
                break;
            case list:

                break;
            case help:
                display_man();
                break;
            case quit:
                close(file);
                run=0;
                break;




        }

    }
}
int main(int argc, char * argv[]){
interact("./readme.md");


}