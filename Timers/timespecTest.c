#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define sleeptime 4500        // Main program runtime in ms

int main(){
    struct timespec tsobj;

    for(int i=0; i<5; i++){
        printf("Iteration : %d\n", i+1);
        
        clock_gettime(CLOCK_MONOTONIC, &tsobj);
        printf("Start : %lld.%.9ld\n", (long long)tsobj.tv_sec, tsobj.tv_nsec);

        tsobj.tv_sec += sleeptime/1000;
        tsobj.tv_nsec += (sleeptime%1000) * 1000000ul;
        if(tsobj.tv_nsec > 1000000000ul){
            tsobj.tv_nsec = tsobj.tv_nsec%1000000000ul;
            tsobj.tv_sec++;
        }
        printf("Set : %lld.%.9ld\n", (long long)tsobj.tv_sec, tsobj.tv_nsec);

        printf("Sleeping for set time..\n\n");
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tsobj, 0);  // sleeps for set period
    }

    return 0;
}