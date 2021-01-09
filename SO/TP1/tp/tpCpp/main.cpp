#include "functions.h"


int main(int argc, char const *argv[]) {

    int numCharacters = 8;
    pthread_t cenouras[numCharacters];

    int ids[numCharacters];
    for (int i = 0; i < numCharacters; i++) {
        ids[i] = i;
    }

    for (int i = 0; i < numCharacters; i++) {
        pthread_create(&(cenouras[i]), NULL, bananinha, &ids[i]);
    }

    for(int i = 0; i < numCharacters; i++) {
        pthread_join(cenouras[i], NULL);
    }
    
    
    return 0;
}
