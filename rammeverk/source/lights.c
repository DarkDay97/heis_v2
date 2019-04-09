#include "lights.h"

const int LIGHT_SIZE = 10;
static int lights[LIGHT_SIZE] = {0};

void lights_set_lights_command(){
    int c;
    for (c = 0; c < N_FLOORS; c++){
        if(elev_get_button_signal(BUTTON_COMMAND, c)){
            lights[c*3] = 1;
        }
    }
}

void lights_set_lights_up(){
    int u;
    for (u = 0; u < N_FLOORS - 1; u++){
        if(elev_get_button_signal(BUTTON_CALL_UP, u)){
            lights[1 + (u*3)] = 1;
        }
    }
}

void lights_set_lights_down(){
    int d;
    for (d = 1; d < N_FLOORS; d++){
        if(elev_get_button_signal(BUTTON_CALL_DOWN, d)){
            lights[(d*3)-1] = 1;
        }
    }
}

void lights_set_lights(){
     lights_set_lights_command();
     lights_set_lights_up();
     lights_set_lights_down();
}

void lights_reset_lights(int floor){
    switch (floor)
    {
        case 0:
            lights[0] = 0;
            lights[1] = 0;
            break;
    
        case 1:
            lights[2] = 0;
            lights[3] = 0;
            lights[4] = 0;
            break;
        
        case 2:
            lights[5] = 0;
            lights[6] = 0;
            lights[7] = 0;
            break;

        case 3:
            lights[8] = 0;
            lights[9] = 0;
            break;

        default:
            break;
    }
}

void lights_reset_all_lights(){
    int i;
    for (i = 0; i < N_FLOORS; i++){
        lights_reset_lights(i);
    }
}

void lights_update_ordering_lights(){

    int i;
    for (i = 0; i < N_FLOORS; i++){
        elev_set_button_lamp(BUTTON_COMMAND, i, lights[i*3]);
    }
    for (i = 0; i < N_FLOORS - 1; i++){
        elev_set_button_lamp(BUTTON_CALL_UP, i, lights[(i*3) + 1]);
    }
    for (i = 1; i < N_FLOORS; i++){
        elev_set_button_lamp(BUTTON_CALL_DOWN, i, lights[(i*3) - 1]);
    }
}