//
// Created by przemek on 12.04.17.
//



#include "slave.h"

Point generate_point(){
    int x = rand()%(RE_MAX-RE_MIN)+RE_MIN;
    int y = rand()%(IM_MAX-IM_MIN)+IM_MIN;
    Point p;
    p.x=x/10000.0;
    p.y=y/10000.0;
    p.abs = sqrt(p.x*p.x + p.y*p.y);
    p.sina = p.y/p.abs;
    p.cosa = p.x/p.abs;

    return p;

}
void display_complex(Point p){
    printf("%f+i*(%f) iter: %d\n",p.x,p.y,p.iter);

}

int count_iter(Point p, int max){
    int i=0;
    double complex zn = 0.0;
    double complex c=p.x+p.y*I;
    for(i=0; i<max && cabs(zn)<2.0; i++){
        zn=cpow(zn, 2.0)+c;
    }
    return i;


}
int main(int argc, char* argv[]){
    char bfr[50];
    int BFR_LEN =50;

    srand(time(NULL));
    char buffer[50];
    if (argc<4) printf("usage: slave [path to named pipe] [amount of complex number to generate] [max limit of iterations]\n");
    else{
        char PATH[100];
        strcpy(PATH,argv[1]);
        int pipe=open(PATH, O_WRONLY);
        if(pipe<0) {
            fprintf(stderr, "error while opening pipe\n");
            EXIT_FAILURE;
        }


        int N=atoi(argv[2]);
        int K=atoi(argv[3]);
        int iter, j;
        Point rand;



        for (j = 0; j<N; j++){

            rand = generate_point();
            rand.iter=count_iter(rand,K);
            sprintf(buffer,"%lf %lf %d\n",rand.x,rand.y,rand.iter);
            write(pipe, buffer,50);
        }

    }

}



