// compile this code with -lm as param
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define NUM_THREADS 4
#define NUM_POINTS 100000000
#define R 1

double global_sum[NUM_THREADS];

void *aThread(void *arg){
    long uid = (long)arg;
    long terms = NUM_POINTS/NUM_THREADS;
    int seed = pthread_self();
    int b = 0;

    for(int i = 0; i < terms; i++){
        double x = (double)rand_r(&seed)/RAND_MAX*(R*2) - R;
        double y = (double)rand_r(&seed)/RAND_MAX*(R*2) - R;
        if(sqrt(x*x + y*y) <= R)
            b++;
    }

    global_sum[uid] = (((float)b)/terms)/NUM_THREADS;
    printf("thread %ld value: %.20g\n", uid, global_sum[uid]);
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    double sum = 0.0;
    for(int i = 0; i < NUM_THREADS; i++){
        int ret = pthread_create(&threads[i], NULL, aThread, (void *)i);
        if(ret){
            printf("Error: pthread_create error, status code: %d", ret);
            exit(ret);
        }
    }


    for(int i = 0; i < NUM_THREADS; i++){
        int ret = pthread_join(threads[i], NULL);
        if(ret){
            printf("Error: pthread_join error, status code: %d", ret);
            exit(ret);
        }
        sum += global_sum[i];
    }
    double pi_approx = sum*4;
    printf("%.20g",pi_approx);
    printf("\n%.20g\n", M_PI);
    return 0;
}