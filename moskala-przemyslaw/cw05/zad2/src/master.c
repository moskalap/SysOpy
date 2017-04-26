//
// Created by przemek on 12.04.17.
//

#include "master.h"


int scale_x(double x, int R) { //x <
    x = x + 2; //x<-0,3
    x = x / 3;
    x = x * R;
    return (int) x;

}

int scale_y(double y, int R) {
    y = y + 1;//x<-0,2
    y = y / 2;
    y = y * R;
    return (int) y;

}

void process(char *buff, int R, int **T) {

    int scaled_x, scaled_y;
    double x = atof(strtok(buff, " "));
    double y = atof(strtok(NULL, " "));
    int i = atoi(strtok(NULL, " "));
    scaled_x = scale_x(x, R);
    scaled_y = scale_y(y, R);
    T[scaled_x][scaled_y] = i;


}

int main(int argc, char *argv[]) {
    char PATH[50];
    char buff[50];

    int fd;

    if (argc < 2) {
        printf("usage [FIFO_NAME] [R]\n");
        EXIT_FAILURE;
    } else {
        int R = atoi(argv[2]);
        strcpy(PATH, argv[1]);


        int **T;

        T = malloc(R * sizeof(int *));
        for (int i = 0; i < R; i++) {
            T[i] = malloc(R * sizeof(int));
        }


        for (int i = 0; i < R; i++)
            for (int j = 0; j < R; j++)
                T[i][j] = 0;

        int fifo = mkfifo(PATH, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
        sleep(15); //Time for user to run slaves
        if (fifo == 0) {

            if ((fd = open(PATH, O_RDONLY)) < 0) {
                fprintf(stderr, "error while opening fifo.\n");
            } else {
                while (read(fd, buff, 50)) {
                    process(buff, R, T);
                }
            }
            remove("data");
            int out = open("data", O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

            char buffer[20];
            for (int x = 0; x < R; x++)
                for (int y = 0; y < R; y++) {
                    sprintf(buffer, "%d %d %d\n", x, y, T[x][y]);
                    write(out, buffer, strlen(buffer));
                }                    //printf("%d %d %d\n",x,y,T[x][y]);



            FILE *plotter = popen("gnuplot", "w");
            fprintf(plotter, "set view map\n");
            fprintf(plotter, "set xrange[0:%d]\n", R - 1);
            fprintf(plotter, "set yrange[0:%d]\n", R - 1);
            fprintf(plotter, "plot 'data' with image\n");

            fflush(plotter);
            getc(stdin);
            fclose(plotter);


        } else {
            fprintf(stderr, "error while creating fifo.\n");
            unlink(PATH);
            EXIT_FAILURE;
        }
    }
    return 0;

}
