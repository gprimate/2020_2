#include "functions.h"
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;

int timeDoingOtherThings () {
    double time = drand48();
    time = time * 4 + 3;
    return (int) time;
}


std::string getName(int id) {
    switch (id) {
        
    case 0:
        return "Sheldon";
    case 1:
        return "Howard";
    case 2:
        return "Leonard";
    case 3:
        return "Stuart";
    case 4:
        return "Kripke";
    case 5:
        return "Amy";
    case 6:
        return "Bernadette";
    case 7: 
        return "Penny";  

    default:
        return NULL;
    }
}


void * bananinha(void * pessego) {
    pthread_mutex_lock(&mutex);

    int *teste = (int *) pessego; 
    int id = *teste;

    warmUpSomething(id);
    eat(id);

    pthread_mutex_unlock(&mutex);

    return NULL;
}


void eat(int id) {
    std::cout << getName(id) << " vai comer" << std::endl;
    sleep(timeDoingOtherThings());
}


void warmUpSomething(int id) {
    std::cout << getName(id) << " começa a esquertar algo" << std::endl;
}