#include "functions.h"


int main(int argc, char const *argv[]) {
    
    int numCharacters = 8;
    int numIterations = 2;

    pthread_t cenouras[numCharacters];

    int ids[numCharacters];
    ThreadInfo info[numCharacters];

    for (int i = 0; i < numCharacters; i++) {
        info[i].id = i;
        info[i].numIter = numIterations;

    }

    for (int i = 0; i < numCharacters; i++) {
        pthread_create(&(cenouras[i]), NULL, bananinha, &info[i]);
    }

    for(int i = 0; i < numCharacters; i++) {
        pthread_join(cenouras[i], NULL);
    }
    
    
    
    return 0;
}
