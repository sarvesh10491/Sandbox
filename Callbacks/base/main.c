#include <stdio.h>
#include <stdint.h>
#include "callback.h"

uint8_t var = 0;
 
// Custom callback function definition
void my_callback(void){
    printf("Inside callback handler..\n");
    var++;
}

int main(){
    printf("Test program to implement callback functionality.\n\n");

    // Initialize function pointer to custom my_callback()
    callback_t ptr_my_callback = my_callback;               

    // Register our callback function
    printf("Registering my_callback() with callback_t..\n");
    register_callback(ptr_my_callback);

    // Waiting for callback trigger.
    while(!var);     

    printf("Inside main program\n");

    return 0;
}