#include <stdio.h>
#include <unistd.h>
#include "callback.h"
 
// Registration of callback function which gets triggered after 5 sec elapses
void register_callback(callback_t ptr_reg_callback){
    printf("Callback registered..\n");
    printf("Sleeping 5 sec & then registered callback function will trigger..\n");
    usleep(5000000);

    // calling registred callback function
    (*ptr_reg_callback)();                                  
}