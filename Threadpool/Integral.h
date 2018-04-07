//
// Created by SergeyN on 30.03.2018.
//
#include <math.h>
#ifndef UNTITLED7_INTEGRAL_H
#define UNTITLED7_INTEGRAL_H
double funcVal(double a) {
    return a*a;     //a*a*tan(a)*exp(a) - sin(a)*cos(tan(sqrt(a)))
}

double calculate(double left_edge, double right_edge, int step_func) {
    double var_res = 0;
    double step_new = (right_edge-left_edge) / step_func;
    for(int i = 1; i <= step_func; i++){
        var_res += step_new * funcVal(left_edge+i*step_new);
    }
    return var_res;
}
#endif //UNTITLED7_INTEGRAL_H
