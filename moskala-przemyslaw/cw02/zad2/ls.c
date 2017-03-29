#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define DT_DIR      0x4

const char* SLASH = "/";
const int BFR = 1024;
int MAX=0;
/**
 * Displays info about file
 * @param fpath - path to file
 * @param sb - stats about file
 * @param tflag -flags for nftw
 * @param ftwbuf  -buffor for nftw
 * @return
 */

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){

    if(sb->st_size>MAX && sb->st_mode!=S_IFDIR) return 0;
    printf( (S_ISDIR(sb->st_mode)) ? "d" : "-");
    printf( (sb->st_mode & S_IRUSR) ? "r" : "-");
    printf( (sb->st_mode & S_IWUSR) ? "w" : "-");
    printf( (sb->st_mode & S_IXUSR) ? "x" : "-");
    printf( (sb->st_mode & S_IRGRP) ? "r" : "-");
    printf( (sb->st_mode & S_IWGRP) ? "w" : "-");
    printf( (sb->st_mode & S_IXGRP) ? "x" : "-");
    printf( (sb->st_mode & S_IROTH) ? "r" : "-"); //read other
    printf( (sb->st_mode & S_IWOTH) ? "w" : "-"); //write other
    printf( (sb->st_mode & S_IXOTH) ? "x" : "-");

    printf("\t");
    char* t=ctime(&(sb->st_mtime));
    t[strlen(t)-1]='\t';
    printf( ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, t);
    printf(ANSI_COLOR_YELLOW "%u bytes\t" ANSI_COLOR_RESET, (unsigned int)sb->st_size);

    printf(ANSI_COLOR_MAGENTA "\t %-40s \n" ANSI_COLOR_RESET,  fpath);
    return 0;           /* To tell nftw() to continue */
}
/**
 * Exploring a path givean as argument
 * @param root_path
 * @param bytes
 */
void search(char* root_path, char * child,  int bytes){
    char root_cpy[BFR];
    strcpy(root_cpy, root_path);

    if(child){
        if(strcmp(child,".") == 0) return;
        if(strcmp(child,"..") == 0) return;
        strcat(root_cpy, SLASH);
        strcat(root_cpy, child);
    }
    DIR * actual=opendir(root_cpy);
    if (!actual) return;
    struct dirent * explorer;
    char path[BFR];
    while(explorer=readdir(actual)) {
        strcpy(path, root_cpy);
        strcat(path, SLASH);
        strcat(path, explorer->d_name);
        if (explorer->d_type == DT_DIR)
            search(root_cpy, explorer->d_name, bytes);

        struct stat *fileStat=malloc(sizeof(struct stat));
        stat(path, fileStat);
        display_info(path, fileStat, 0, NULL);
    }
}
/**
 * Exploring a path using nwfd
 * @param argc
 * @param path
 */
void search_nftw(int argc, char *path){
    MAX=argc;
    int flags = 0;
    flags |= FTW_DEPTH;
    flags |= FTW_PHYS;
    if (nftw(path, display_info, 20, flags)
        == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void display_man(){
    printf("\nUse:\n\t-nftw\t'directory-path'\tMAX_BYTES\t for search using nftw(),\n\t-recur\t'directory-path'\tMAX_BYTES\t for search using recuresion.\n");
}
int main(int argc, char* argv[]){


    if(argc<4){
       display_man();
    }else{
        char *option=argv[1];
        char *pah=argv[2];
        char a[1024];
        strcpy(a,pah);
        if(strcmp(option, "-nftw")==0){
            search_nftw(atoi(argv[3]), pah);

        }else {
            if (strcmp(option, "-recur")==0){
                MAX=atoi(argv[3]);
                search(pah, NULL, atoi(argv[3]));

            }
            else display_man();
        }


    }



}