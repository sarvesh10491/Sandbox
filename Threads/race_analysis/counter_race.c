#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 20
#define NUM_COUNTS  1000000

int counter;

void *thread_func(void *thread_num){
    for(int i=0; i<NUM_COUNTS; i++)
        counter += 1;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int ret;
    pthread_t tid[NUM_THREADS];

    for(int i=0; i<NUM_THREADS; i++){
        ret = pthread_create(&tid[i], NULL, thread_func, (void *)i);
        if(ret){
            printf("ERROR : Return code from pthread_create() is %d\n", ret);
            exit(1);
        }
    }

    // Wait for threads to finish
    for(int i=0; i<NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    printf("Final value of counter is: %d\n", counter);
    pthread_exit(NULL);

    return 0;
}