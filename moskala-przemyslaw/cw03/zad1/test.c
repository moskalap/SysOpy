#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]){
    printf("\nZmienna %s wynosi ", argv[1]);
    printf(getenv(argv[1]));
}