#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>



/**
 * Exploring a path givean as argument
 * @param root_path
 * @param bytes
 */
void search(char* root_path, int bytes){
DIR* root=opendir(root_path);
struct dirent* dent;
    dent=readdir(root);
    while(dent){

        printf("[%s %u]", dent->d_name, dent->d_type);




        dent=readdir(root);
    }
}

int main(int argc, char* argv[]){

    //const char * root_path=argv[1];
   // const int max_bytes=atoi(argv[2]);

    search ("/home/przemek/Dokumenty/", 2);







}