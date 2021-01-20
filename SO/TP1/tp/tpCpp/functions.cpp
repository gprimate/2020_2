#include "functions.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t person[8];

int activeThreads = 0;
bool ovenInUse = false;
int waitList[] = {0,0,0,0,0,0,0,0};

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
        return "Amy";
    case 4:
        return "Bernadette";
    case 5:
        return "Penyy";
    case 6:
        return "Stuart";
    case 7: 
        return "Kripke";  

    default:
        return NULL;
    }
}


void * bananinha(void * pessego) {
    pthread_mutex_lock(&mutex);
    activeThreads++;
    pthread_mutex_unlock(&mutex);

    ThreadInfo *teste = (ThreadInfo *) pessego; 

    wait((*teste).id);
    warmUpSomething((*teste).id);
    
    eat((*teste).id);
    
    return NULL;
}


void eat(int id) {
    std::cout << getName(id) << " vai comer" << std::endl;
    sleep(timeDoingOtherThings());
}


void warmUpSomething(int id) {
    std::cout << getName(id) << " começa a esquertar algo" << std::endl;
}

void wait(int id) {
    pthread_mutex_lock(&mutex);
    std::cout << getName(id) << " quer usar o forno" << std::endl;
    
    if (ovenInUse) {
        waitList[id]++;

        if (id > 5) {
            pthread_cond_wait(&(person)[id], &mutex);

        } else if (checkPartner) {
            pthread_cond_wait(&(person)[(id % 3) + 3], &mutex);
            pthread_cond_wait(&(person)[(id % 3)], &mutex);

        } else {
            pthread_cond_wait(&(person)[(id % 3)], &mutex);
        }
    }
    ovenInUse = true;
    if (waitList[id] == 1) {
        waitList[id] = 0;
    }

    pthread_mutex_unlock(&mutex);
}

bool checkPartner(int id) {
    int idCouple = id % 3;

    if (id < 6 && (waitList[idCouple] != 0 || waitList[idCouple + 3] != 0)) {  
        return true;
       
    } else {
        return false;
    } 
}