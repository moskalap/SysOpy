//
// Created by przemek on 12.04.17.
//


#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include "slave.h"
#include <complex.h>
double complex generate_complex(){
    int x=rand()%((RE_MAX-RE_MIN)*100);
    int y=rand()%((IM_MAX-IM_MIN)*100);
    double xd = (double) x/10000.0+RE_MIN;
    double yd;
    return xd + yd * I;

}
int main(int argc, char* argv[]){
    srand(time(NULL));
    if (argc<4) printf("usage: slave [path to named pipe] [amount of complex number to generate] [max limit of iterations]\n");
    else{
        int pipe = open(argv[1], O_WRONLY);
        int N=atoi(argv[2]);
        int K=atoi(argv[3]);
        int iter, j;
        double complex rand;
        for (j = 0; j<N; j++){
            rand = generate_complex();
            iter = count_iter(g);
            write_to_pipe(pipe,rand,iter);
        }

    }


}
