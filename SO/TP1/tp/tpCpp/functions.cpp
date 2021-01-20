#include "functions.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
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
void eat() {
    sleep(timeDoingOtherThings());
}

void * bananinha(void * pessego) {
    pthread_mutex_lock(&counter);
    activeThreads++;
    pthread_mutex_unlock(&counter);

    ThreadInfo *teste = (ThreadInfo *) pessego; 

    while ((*teste).numIter > 0) {
        wait((*teste).id);
        warmUpSomething((*teste).id);
        freeUser((*teste).id);
        eat();

        (*teste).numIter--;
    }

    pthread_mutex_lock(&counter);
    activeThreads--;
    pthread_mutex_unlock(&counter);

    pthread_exit(NULL);

    return NULL;
}

void warmUpSomething(int id) {
    std::cout << getName(id) << " começa a esquertar algo" << std::endl;
    sleep(1);
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

int coupleId(int id) {
    return (id % 3);
}

int largerPrecedence(int id) {
    int indice;

	if (id == 0) {
		indice = 2;
	} else {
		indice = (id - 1) % 3;
	}

	return indice;
}

int smallerPrecedence(int id) {
    return (id + 1) % 3;
}




void freeUser(int id) {
    pthread_mutex_lock(&mutex);

    std::cout << getName(id) << " vai comer" << std::endl;

    ovenInUse = false;

    if (checkPartner(0) && checkPartner(1) && checkPartner(2)) {
        if (checkPartner(id)) {
            pthread_cond_signal(&(person[coupleId(id) + 3]));
        } 
        
    } else {
        if (id < 6) {
            if (checkPartner(largerPrecedence(id))) {
                pthread_cond_signal(&(person[largerPrecedence(id)]));
			    pthread_cond_signal(&(person[largerPrecedence(id) + 3]));

            } else if (checkPartner(coupleId(id))) {
                pthread_cond_signal(&(person[coupleId(id)]));
			    pthread_cond_signal(&(person[coupleId(id) + 3]));

            } else  {
                pthread_cond_signal(&(person[smallerPrecedence(id)]));
			    pthread_cond_signal(&(person[smallerPrecedence(id) + 3]));
            }
        } else {
            if (id == 6) {
                pthread_cond_signal(&person[7]);
            }
        }
    } 
}  