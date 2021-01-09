#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void * bananinha(void * pessego) {
    int *teste = (int *) pessego; 
    printf("%d \n", *teste);
}

int main(int argc, char const *argv[])
{
    pthread_t cenouras[5];
    int ids[5];
    for (int i = 0; i < 5; i++) {
        ids[i] = i;
    }

    for (int i = 0; i < 5; i++) {
        pthread_create(&(cenouras[i]), NULL, bananinha, &ids[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(cenouras[i], NULL);
    }
    
    /*
    for(int i = 0; i < 25; i++) {
        double a = drand48();
        a *= 3;
        int b = (int) a;
        printf("%d \n", b);
    }
    */
    return 0;
}
