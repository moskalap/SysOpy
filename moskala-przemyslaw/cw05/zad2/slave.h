//
// Created by przemek on 12.04.17.
//

#ifndef CW05Z02_SLAVE_H
#define CW05Z02_SLAVE_H


typedef struct Point{
    double x;
    double y;
    double abs;
    double sina;
    double cosa;
    int iter;
}Point;
const int IM_MAX = 100;
const int IM_MIN = -100;
const int RE_MAX = 100;
const int RE_MIN = -200;

#endif //CW05Z02_SLAVE_H
