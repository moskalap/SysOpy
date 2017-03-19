#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <dirent.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define DT_DIR      0x4




const char* SLASH = "/";


void getFileCreationTime(char *path) {
    struct stat attr;
    stat(path, &attr);
    char* t=ctime(&attr.st_mtime);
    t[strlen(t)-1]='\t';
    printf( "%s", t);
    printf("%u bytes\t", (unsigned int)attr.st_size);

}
/**
 * Exploring a path givean as argument
 * @param root_path
 * @param bytes
 */
void search(char* root_path, char * child,  int bytes){

    if(child)
    {
        if (strcmp(child, ".") == 0) return;
        if (strcmp(child, "..") == 0) return;
        strcat(root_path, child);
        strcat(root_path, SLASH);
    }
    DIR* root=opendir(root_path);
    char * copy = malloc(sizeof(char)*strlen(root_path));
    strcpy(copy,root_path);
    char tmp[1024];
struct dirent* dent;
    dent=readdir(root);
    printf(ANSI_COLOR_BLUE "%s"ANSI_COLOR_RESET, root_path );
    while(dent ){
        strcpy(tmp, root_path);
        //strcat(copy, SLASH);
        getPermission(tmp, dent->d_name);
        getFileCreationTime(tmp);
        printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, dent->d_name );



       if(dent->d_type==DT_DIR) search(root_path,dent->d_name, bytes);
        dent=readdir(root);
    }

    closedir(root);
}
void getPermission(char * root, char * file){

    strcat(root, SLASH);
    strcat(root, file);
        struct stat fileStat;
if(stat(root,&fileStat) < 0) printf("err");
    else{
    printf("\n");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-"); //read other
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-"); //write other
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");//execute other
    printf("\t");

}

}




static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){



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
    printf( "%s", t);
    printf("%u bytes\t", (unsigned int)sb->st_size);

    printf("\t %-40s \n",  fpath);
    return 0;           /* To tell nftw() to continue */
}

void search_nwfd(int argc, char *path){
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



int main(int argc, char* argv[]){


    char* pah=argv[1];
    char a[1024];
    strcpy(a,pah);
    //getPermission(pah);
    //search (a,NULL, 2);
    search_nwfd(2,pah);






}