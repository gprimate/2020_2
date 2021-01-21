#include "functions.h"


int main(int argc, char const *argv[]) {
    
    int numCharacters = 8;
    int numIterations = 5;

    pthread_t cenouras[numCharacters];
    pthread_t raj;

    int ids[numCharacters];
    ThreadArg info[numCharacters];

    for (int i = 0; i < numCharacters; i++) {
        info[i].id = i;
        info[i].num_iteracoes = numIterations;

    }

    for (int i = 0; i < numCharacters; i++) {
        pthread_create(&(cenouras[i]), NULL, inicia_casais, &info[i]);
    }
    pthread_create(&raj, NULL, inicia_raj, NULL);

    for(int i = 0; i < numCharacters; i++) {
        pthread_join(cenouras[i], NULL);
    }
    pthread_join(raj, NULL);
    

    return 0;
}
