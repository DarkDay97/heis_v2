#include "timer.h"

const int LIMIT = 3; //Timelimit
static clock_t = clock_var;

void timer_init(){
    timer_reset();
}

void timer_reset(){
    clock_var = clock();
}

int timer_expired(){
    clock_t clock_now = clock();
    return ((clock_now - clock_var) >= LIMIT * CLOCKS_PER_SEC);
}