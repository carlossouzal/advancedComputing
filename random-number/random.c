// compile this code with -lm as param
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREADS 4

pthread_t threads[NUM_THREADS];
int main_rand;

void *aThread(void *arg){
    long uid = (long)arg;
    int seed = pthread_self();
    int number = rand_r(&seed);

    while(number != main_rand){
        pthread_testcancel();
        number = rand_r(&seed);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        if(uid == i) continue;

        int ret = pthread_cancel(threads[i]);
        if(ret){
            printf("ERROR; return code from pthread_cancel() is %d\n", ret);
            exit(ret);
        }
    }
    
    pthread_exit((void *)uid);
}

int main(){
    unsigned int mainSeed = getpid();
    main_rand = rand_r(&mainSeed);
    long threadWins;

    puts("The number was sorted");

    for(int i = 0; i < NUM_THREADS; i++){
        int ret = pthread_create(&threads[i], NULL, aThread, (void *)i);
        if(ret){
            printf("Error: pthread_create error, status code: %d", ret);
            exit(ret);
        }
    }

    for(int i = 0; i < NUM_THREADS; i++){
        void *returnValue;
        int ret = pthread_join(threads[i], &returnValue);
        if(returnValue != PTHREAD_CANCELED){
            threadWins = (long) returnValue;
        }
    }

    printf("SUCESS: The Thread number %ld, guessed the number\n", threadWins);
    return 0;
}