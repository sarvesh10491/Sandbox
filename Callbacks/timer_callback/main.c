#include <stdio.h>
#include <stdint.h>
#include "timercb.h"

uint8_t var = 0;
 
// Custom callback handler function definition
void handler_func(void){
    printf("Inside callback handler..\n");
    var++;
}

int main(){
    printf("Test program to implement callback\n-----------------------------------\n");

    // Initialize timer callback context instance with handler & user data with timercb_t
    timercb_t timerobj;
    timerobj.callbackHandler = handler_func;
    timerobj.sleeptime = 5000000;               

    // Register callback handler
    register_callback(&timerobj);

    printf("Waiting in main for callback trigger..\n");
    while(!var);     

    printf("Inside main program again..\n");

    return 0;
}