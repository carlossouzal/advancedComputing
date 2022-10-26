#include<stdio.h>
#include<math.h>

int main(){
    long t = 10000000;
    double factor = 1.0;
    double sum = 0.0;

    for(int i = 0; i < t; i++){
        sum += factor / (2*i+1);
        factor = -factor;
    }

    double pi_approx = sum*4;
    printf("%f",pi_approx);
    printf("\n%f\n", M_PI);
    return 0;
}