#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREAD 2

double sum[NUM_THREAD];
long t = 10000000/NUM_THREAD;

void *aThread(void * param){
    long uid = (long) param;
    double factor = (t*uid % 2) ? -1.0 : 1.0;
    double local_sum = 0.0;

    for(int i = t*uid; i < t*(uid+1); i++){
        local_sum += factor / (2*i+1);
        factor = -factor;
    }

    sum[uid] = local_sum;
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREAD];
    double global_sum = 0.0;
    for(int i = 0; i < NUM_THREAD; i++){
        int ret = pthread_create(&threads[i], NULL, aThread, (void *)i);
        if(ret){
            printf("Error: pthread_create error, status code: %d", ret);
            exit(ret);
        }
    }


    for(int i = 0; i < NUM_THREAD; i++){
        int ret = pthread_join(threads[i], NULL);
        if(ret){
            printf("Error: pthread_join error, status code: %d", ret);
            exit(ret);
        }
        global_sum += sum[i];
    }
    double pi_approx = global_sum*4;
    printf("%f",pi_approx);
    printf("\n%f\n", M_PI);
    return 0;
}