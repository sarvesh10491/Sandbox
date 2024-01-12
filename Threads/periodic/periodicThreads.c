#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <semaphore.h>
#include <time.h>
#include <sched.h>

//######################################################
//
// Global variable & functions declarations area
//
//######################################################

#define main_runtime 5000        // Main program runtime in ms

pthread_cond_t start;            // Condition variable for start sync
pthread_mutex_t start_mut;       // Mutex used in conjunction with condition variables

int stop_flag = 1;

//===============================
// Thread description parameters
//===============================

#define P_SENDER_THREAD 4               // Number of periodic threads

const int t_periods[] = {25,40,75,60};  // Periodic thread periods(ms)

//==============================
// Thread priority attributes
//==============================
#define P_SENDER_PRIO 40

//==============================
// Thread context struct
//==============================
struct t_data{
    int t_id;
    int t_num;
    int t_runs;
};

//######################################################
//
// Thread functions
//
//######################################################

// Thread function for periodic threads
//---------------------------------------------
void *periodic_thread_func(void *argptr){
    // Thread function computations
    struct t_data *tptr;
    tptr = (struct t_data *)argptr;

    printf("Periodic thread invoked : %d with period %d us\n", tptr->t_num, t_periods[tptr->t_num]);

    // Each thread waits here till conditional variable broadcasts
    pthread_mutex_lock(&start_mut);
    int cret = pthread_cond_wait(&start,&start_mut);            // All threads waiting on start condition variable to start simultaneously
    if(cret!=0)
        printf("Periodic condition variable error %d\n",cret);
    pthread_mutex_unlock(&start_mut);
    
    struct timespec base,period;
    period.tv_sec = (t_periods[tptr->t_num])/1000;
    period.tv_nsec = (t_periods[tptr->t_num])*1000000ul;

    clock_gettime(CLOCK_MONOTONIC,&base);

    while(stop_flag){
        // Arm for next timer deadline
        base.tv_sec += period.tv_sec;
        base.tv_nsec += period.tv_nsec;
        if(base.tv_nsec > 1000000000ul){
            base.tv_nsec = base.tv_nsec%1000000000ul;
            base.tv_sec++;
        }

        // Thread compute
        printf("Periodic thread running : %d\n", tptr->t_num);
        tptr->t_runs += 1;

        clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&base,0);     // Takes time provided in time object as absolue time and thread goes to sleep
    }
    printf("Periodic thread %d was called %d times.\n", tptr->t_num, tptr->t_runs);   

    pthread_exit(NULL);
}

//######################################################
//
// Main
//
//######################################################


int main(int argc, char const *argv[]){
    //=========================================
    // Thread Initializations
    //=========================================
    int tret = -1;

    pthread_t P_tid[P_SENDER_THREAD];

    pthread_attr_t fifo_attr;
    pthread_attr_init(&fifo_attr);
    // pthread_attr_setinheritsched(&fifo_attr,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&fifo_attr,SCHED_FIFO);

    struct sched_param param;

    //=========================================
    // Thread conditional variables
    //=========================================
    int cret = pthread_cond_init(&start,NULL);            // initialising condition variables
    if(cret!=0)
        printf("Start condn init error %d\n",cret);

    pthread_mutex_init(&start_mut,NULL);                 // Initialising condition variable mutexes(thread mutex are different)


    //=========================================
    // Set CPU affinity
    //=========================================
    // Bitset where each bit represents a CPU.
    cpu_set_t cpuset;
    // Initializes the CPU set to be the empty set.
    CPU_ZERO(&cpuset);
    // CPU_SET: This macro adds cpu to the CPU set.
    CPU_SET(0, &cpuset);


    //=========================================
    // Threads creation
    //=========================================

    //Creating Periodic threads
    //----------------------------------
    param.sched_priority = P_SENDER_PRIO;
    pthread_attr_setschedparam(&fifo_attr, &param);

    struct t_data *dptr[P_SENDER_THREAD];

    for(int i=0; i<P_SENDER_THREAD; i++){
        dptr[i] = (struct t_data *)malloc(sizeof(struct t_data));
        dptr[i]->t_id = P_tid[i];
        dptr[i]->t_num = i;
        dptr[i]->t_runs = 0;

        tret = pthread_create(&P_tid[i], &fifo_attr, periodic_thread_func, (void *)dptr[i]);
        pthread_setaffinity_np(P_tid[i], sizeof(cpu_set_t), &cpuset);
        if(tret){
            printf("ERROR. Return code from pthread_create() is %d\n", tret);
            exit(-1);
        }
    }

    // Conditional variable & setting main runtime
    //=============================================
    usleep(1000);                                      // allowing all threads to be created and waitinng on condition variable before broadcasting
    pthread_cond_broadcast(&start);                    // all threads start running


    struct timespec mainstop;                          // timespec variable for calculating time for terminating main program
    clock_gettime(CLOCK_MONOTONIC, &mainstop);         // gets current time

    mainstop.tv_sec += main_runtime/1000;
    mainstop.tv_nsec += (main_runtime%1000) * 1000000ul;
    if(mainstop.tv_nsec > 1000000000ul){
        mainstop.tv_nsec = mainstop.tv_nsec%1000000000ul;
        mainstop.tv_sec++;
    }

    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &mainstop, 0);  // sleeps for total program period

    //=========================================
    // Threads termination
    //=========================================
    printf("\nExecuted for %d ms\n", main_runtime);

    stop_flag = 0;
    pthread_cond_destroy(&start);

    for(int i=0; i<P_SENDER_THREAD; i++){
        pthread_join(P_tid[i],NULL);
        free(dptr[i]);
    }

    printf("\nTerminated successfully.\n");

    return 0;
}

//######################################################
//
// End of Main
//
//######################################################