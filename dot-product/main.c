#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREAD 1
#define VECTOR_LENGTH 268435456

int GLOBAL_a[VECTOR_LENGTH];
int GLOBAL_b[VECTOR_LENGTH];
long GLOBAL_product[(NUM_THREAD + 1)];

void *aThread(void * param){
    long uid = (long) param;
    int worker_load = uid ? VECTOR_LENGTH/(NUM_THREAD+1) : VECTOR_LENGTH/(NUM_THREAD+1) + 1;
    int worker_init = uid ? worker_load*uid + 1 : worker_load*uid;
    int worker_end  = worker_init+worker_load;

    for(int i = worker_init; i < worker_end; i++)
        GLOBAL_product[uid] += GLOBAL_a[i]*GLOBAL_b[i];

    printf("Hi, I'm thread %ld, my worker slice starts at %d and finish at %d index. And my worker slice have %d length. My result was %ld\n", uid, worker_init, worker_end, worker_load, GLOBAL_product[uid]);
    if(uid)
        pthread_exit(NULL);
}

void vector_init(){
    for(int i= 0; i < VECTOR_LENGTH; i++){
        GLOBAL_a[i] = 1;
        GLOBAL_b[i] = 2;
    }
}

int main(){
    pthread_t threads[NUM_THREAD];
    long global_sum;
    vector_init();

    for(int i = 0; i < NUM_THREAD; i++){
        int ret = pthread_create(&threads[i], NULL, aThread, (void *)(i+1));
        if(ret){
            printf("Error: pthread_create error, status code: %d", ret);
            exit(ret);
        }
    }
    aThread((void *) 0);
    global_sum = GLOBAL_product[0];
    for(int i = 0; i < NUM_THREAD; i++){
        int ret = pthread_join(threads[i], NULL);
        if(ret){
            printf("Error: pthread_join error, status code: %d", ret);
            exit(ret);
        }

        global_sum += GLOBAL_product[i+1];
    }

    printf("The dot product is %ld\n",global_sum);
    return 0;
}