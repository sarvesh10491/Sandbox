#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include "timercb.h"

#define TIMERCB_THREAD_PRIO 40
 
void *timercb_thread_func(void *timerCtx){
    // Timer cb thread function computations
    timercb_t *tptr;
    tptr = (timercb_t *)timerCtx;

    usleep(tptr->sleeptime);

    // Calling registered handler callback function
    printf("Callback triggered..\n");
    (*(tptr->callbackHandler))();   

    pthread_exit(NULL);
}


void register_callback(timercb_t *timerCtx){
    int tret = -1;
    pthread_t tid;
    pthread_attr_t fifo_attr;
    struct sched_param param;
    pthread_attr_init(&fifo_attr);
    pthread_attr_setschedpolicy(&fifo_attr, SCHED_FIFO);
    param.sched_priority = TIMERCB_THREAD_PRIO;
    pthread_attr_setschedparam(&fifo_attr, &param);
    
    tret = pthread_create(&tid, &fifo_attr, timercb_thread_func, (void *)timerCtx);
    if(tret){
        printf("ERROR. Return code from pthread_create() is %d\n", tret);
        exit(-1);
    }

    printf("Callback registered..\n");                            
}
