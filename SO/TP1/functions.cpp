#include "functions.h"


int espera[8] = {0, 0, 0, 0, 0, 0, 0, 0};
pthread_cond_t pessoas[8];
int esperaCasal[3] = {0, 0, 0};
std::array<std::pair<int, int>, 3> precedenciaCasal = { {{-1, -1}, {-1, -1}, {-1, -1}} };
int casalInterrompido[3] = {0, 0, 0};
int threads_ativas = 0;

int forno = 0;

pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t contador = PTHREAD_MUTEX_INITIALIZER;

bool casal() {
	if(esperaCasal[0] || esperaCasal[1] || esperaCasal[2]) {
		return true;
	} 
	else {
		return false;
	}
}

bool deadlock(){
	if((espera[0] || espera[1]) && (espera[2] || espera[3]) && (espera[4] || espera[5])) {
		if((esperaCasal[0] && esperaCasal[1] && esperaCasal[2]) || !casal()) {
			return true;
		}
	}
	return false;
}


int getCasalId(int id){
	return id / 2;
}

int getOutroMembroCasal(int id){
	return ((id % 2 == 0) ? id + 1 : id - 1);
}

void enfileirarCasal(int id) {
	int casalId = getCasalId(id);
	esperaCasal[casalId] = 1;

	precedenciaCasal[casalId].first = getOutroMembroCasal(id);
	precedenciaCasal[casalId].second = id;
}

void checkAndUpdateCasal(int id) {
	int casalId = getCasalId(id);
	if(esperaCasal[casalId]){
		if(precedenciaCasal[casalId].second != -2){ 
			precedenciaCasal[casalId].first = precedenciaCasal[casalId].second;
			precedenciaCasal[casalId].second = -2;
		} else {
			esperaCasal[casalId] = 0;
		}	
	}
}

int next() {
	if (deadlock()){
		return -1;
	}

	if(casal()){
		if((esperaCasal[0]) && (esperaCasal[1]) && (!esperaCasal[2])) {
			return precedenciaCasal[0].first;
		}
		else if((esperaCasal[0]) && (!esperaCasal[1]) && (esperaCasal[2])) {
			return precedenciaCasal[2].first;
		}
		else if((esperaCasal[0]) && (!esperaCasal[1]) && (!esperaCasal[2])) {
			return precedenciaCasal[0].first;
		} 
		else if((!esperaCasal[0]) && (esperaCasal[1]) && (esperaCasal[2])) {
			return precedenciaCasal[1].first;
		}
		else if((!esperaCasal[0]) && (!esperaCasal[1]) && (esperaCasal[2])) {
			return precedenciaCasal[2].first;
		}
		else if((!esperaCasal[0]) && (esperaCasal[1]) && (!esperaCasal[2])) {
			return precedenciaCasal[1].first;
		}
	} else {
		if((espera[0] || espera[1]) && !(espera[4] || espera[5])) {
			return (espera[0] == 1? 0 : 1);
		}
		else if((espera[2] || espera[3]) && !(espera[0] || espera[1])) {
			return (espera[2] == 1? 2 : 3);
		}
		else if((espera[4] || espera[5]) && !(espera[2] || espera[3])) {
			return (espera[4] == 1? 4: 5);
		} 
		else if(espera[6] == 1) {
			return 6;
		}
		else if(espera[7] == 1) {
			return 7;
		}
	}
	return -2;
}


int timeDoingOtherThings() {
    double time = drand48();
    time = time * 4 + 3;
    return (int) time;
}

void verifica() {
	/*
	std::cout << "verificnado" << std::endl;
	for (int i = 0; i < 8; i++) {
		std::cout << espera[i] << "\t";
	
	}
	std::cout << "\n";
	*/
	if (deadlock() && forno == 0) {

		int chosen;

		do {
			double id = drand48();
        	id = id * 6;
			chosen = (int) id;
		} while (!checkId(chosen));

		pthread_mutex_lock(&monitor);

		pthread_cond_signal(&pessoas[chosen]);
		std::cout << "Raj detectou um deadlock, liberando " << getName(chosen) << "." << std::endl;
		pthread_mutex_unlock(&monitor);
	}
}


void esperar(int id) {
	pthread_mutex_lock(&monitor);
	espera[id] = 1;

	std::cout << getName(id) << " quer usar o forno" << std::endl; // adicionei essa linha

	if(forno != 0){
		if(id < 6) {
			if(espera[getOutroMembroCasal(id)]){
				enfileirarCasal(id);
			}
		}
		pthread_cond_wait(&pessoas[id], &monitor);
	}

	forno = 1;

	if (espera[id] == 1) {
		espera[id] = 0;
	}
	
	pthread_mutex_unlock(&monitor);
}

void esquentar(int id) {

	std::cout << getName(id) << " começa a esquentar algo"<< std::endl;
	sleep(1);
}

void liberar(int id) {
	pthread_mutex_lock(&monitor);
	std::cout << getName(id) << " vai comer" << std::endl;
	forno = 0;

	if (id < 6) {
		checkAndUpdateCasal(id);
	}

	int nextUse = next();
	std::cout << nextUse <<std::endl;

	if (nextUse >= 0) {
		pthread_cond_signal(&pessoas[nextUse]);

	}

	pthread_mutex_unlock(&monitor);
}

void comer() {
	int sleepTime = timeDoingOtherThings();
	//std::cout << sleepTime << std::endl;
	sleep(sleepTime);
}

void *inicia_casais(void *arg) {

	pthread_mutex_lock(&contador);
	threads_ativas++;
	pthread_mutex_unlock(&contador);

	ThreadArg *t = (ThreadArg *) arg;

	while ((*t).num_iteracoes > 0) {

		esperar((*t).id);
		esquentar((*t).id);
		liberar((*t).id);
		comer();

		(*t).num_iteracoes--;
	}

	pthread_mutex_lock(&contador);
	threads_ativas--;
	pthread_mutex_unlock(&contador);

	pthread_exit(NULL );
}

void *inicia_raj(void *arg) {

	while (threads_ativas > 0) {
		verifica();
		sleep(5);
	}

	pthread_exit(NULL );

}

std::string getName(int id) {
    switch (id) {
        
    case 0:
        return "Sheldon";
    case 1:
        return "Amy";
    case 2:
        return "Howard";
    case 3:
        return "Bernadette";
    case 4:
        return "Leonard";
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

bool checkId(int id) {
	return (espera[id] ? true : false);
}