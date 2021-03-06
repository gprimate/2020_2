#include "functions.h"


int espera[8] = {0, 0, 0, 0, 0, 0, 0, 0};
pthread_cond_t pessoas[8];
int esperaCasal[3] = {0, 0, 0};
int esperaCasalIncompleto[3] = {0, 0, 0};
std::array<std::pair<int, int>, 3> precedenciaCasal = { {{-1, -1}, {-1, -1}, {-1, -1}} };


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

bool casalIncompleto() {
	if(esperaCasalIncompleto[0] || esperaCasalIncompleto[1] || esperaCasalIncompleto[2]) {
		return true;
	}
	return false;
}

int countCasalIncompleto() {
	int count = 0;
	for(int i = 0; i < 3; i++){
		if(esperaCasalIncompleto[i]){
			count++;
		}
	}
	return count;
}

// Função responsável por verificar se existe um deadlock
bool deadlock(){

	// Se há pelo menos um membro de cada casal esperando, existe a possibilidade de Deadlock
	if((espera[0] || espera[1]) && (espera[2] || espera[3]) && (espera[4] || espera[5])) { 

		//Se há três casais completos ocorre Deadlock
		if(esperaCasal[0] && esperaCasal[1] && esperaCasal[2]){ 
			return true;

		//Se não há nenhum casal completo, pode ocorrer Deadlock se nenhum ou todos tiverem prioridade de casal incompleto
		} else if (casalIncompleto()) {
			if(countCasalIncompleto() == 3){
				return true;
			} else {
				return false;
			}
		}

	}
	return false;
}

// Funcão que retorna o id do casal
int getCasalId(int id){
	return id / 2;
}

//Função que retorna o id do cônjuge de um personagem
int getOutroMembroCasal(int id){
	return ((id % 2 == 0) ? id + 1 : id - 1);
}

//Função responsável por enfilerar um casal
void enfileirarCasal(int id) {
	int casalId = getCasalId(id);
	esperaCasal[casalId] = 1;
	esperaCasalIncompleto[casalId] = 0;

	precedenciaCasal[casalId].first = getOutroMembroCasal(id);
	precedenciaCasal[casalId].second = id;
}


//Função que atualiza os casais quando um novo personagem é liberado
void checkAndUpdateCasal(int id) {
	int casalId = getCasalId(id);
	if(esperaCasal[casalId]){
		precedenciaCasal[casalId].first = precedenciaCasal[casalId].second;
		esperaCasalIncompleto[casalId] = 1;
	} else if(esperaCasalIncompleto[casalId]){
		esperaCasalIncompleto[casalId] = 0;
	}

	esperaCasal[casalId] = 0;
}

// Função responsável por verificar quem será o próximo a utilizar o forno
int next(int id) {
	if (deadlock()){
		return -1;
	}

	if (casal()){

		if ((esperaCasal[0]) && (esperaCasal[1]) && (!esperaCasal[2])){
			return precedenciaCasal[0].first;

		} else if ((esperaCasal[0]) && (!esperaCasal[1]) && (esperaCasal[2])){
			return precedenciaCasal[2].first;

		} else if ((esperaCasal[0]) && (!esperaCasal[1]) && (!esperaCasal[2])){
			return precedenciaCasal[0].first;

		} else if (!(esperaCasal[0]) && (esperaCasal[1]) && (esperaCasal[2])){
			return precedenciaCasal[1].first;

		} else if (!(esperaCasal[0]) && (!esperaCasal[1]) && (esperaCasal[2])){
			return precedenciaCasal[2].first;

		} else if (!(esperaCasal[0]) && (esperaCasal[1]) && (!esperaCasal[2])){
			return precedenciaCasal[1].first;
		}

	} else if(casalIncompleto()){

		if((esperaCasalIncompleto[0]) && (esperaCasalIncompleto[1]) && (!esperaCasalIncompleto[2])) {
			return precedenciaCasal[0].first;
			
		}
		else if((esperaCasalIncompleto[0]) && (!esperaCasalIncompleto[1]) && (esperaCasalIncompleto[2])) {
			return precedenciaCasal[2].first;
			
		}
		else if((esperaCasalIncompleto[0]) && (!esperaCasalIncompleto[1]) && (!esperaCasalIncompleto[2])) {
			return precedenciaCasal[0].first;
			
		} 
		else if((!esperaCasalIncompleto[0]) && (esperaCasalIncompleto[1]) && (esperaCasalIncompleto[2])) {
			return precedenciaCasal[1].first;
			
		}
		else if((!esperaCasalIncompleto[0]) && (!esperaCasalIncompleto[1]) && (esperaCasalIncompleto[2])) {
			return precedenciaCasal[2].first;
			
		}
		else if((!esperaCasalIncompleto[0]) && (esperaCasalIncompleto[1]) && (!esperaCasalIncompleto[2])) {
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

//Função que calcula o tempo que os personagens gastam comendon e trabalhando
int timeDoingOtherThings() {
    srand (time(NULL));
    int time = rand() % 3 + 1;
    return time;
}


void verifica() {
	if (deadlock() && forno == 0) {

		int chosen;
		srand(time(NULL));

		do {
			chosen = rand() % 6;
		} while (!espera[chosen]);


		pthread_mutex_lock(&monitor);

		pthread_cond_signal(&pessoas[chosen]);
		std::cout << "Raj detectou um deadlock, liberando " << getName(chosen) << std::endl;

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
	std::cout << getName(id) << " começa a esquentar algo" << std::endl;
	sleep(1);
}

void liberar(int id) {
	pthread_mutex_lock(&monitor);

	std::cout << getName(id) << " vai comer" << std::endl;
	forno = 0;

	if (id < 6) {
		checkAndUpdateCasal(id);
	}

	int nextUse = next(id);

	if (nextUse >= 0) {
		pthread_cond_signal(&pessoas[nextUse]);

	}

	pthread_mutex_unlock(&monitor);
}

void comer() {
	int sleepTime = timeDoingOtherThings();
	sleep(sleepTime);
}

void trabalhar(int id) {
	std::stringstream message;
	message << getName(id) << " voltou ao trabalho" << std::endl;
	std::cout << message.str();

	int sleepTime = timeDoingOtherThings();
	sleep(sleepTime);
}

void *inicia_casais(void *arg) {

	pthread_mutex_lock(&contador);
	threads_ativas++;
	pthread_mutex_unlock(&contador);

	ThreadArg *t = (ThreadArg *) arg;
	srand(time(NULL));
	sleep(rand() % 3);

	while ((*t).num_iteracoes > 0) {

		esperar((*t).id);
		esquentar((*t).id);
		liberar((*t).id);
		comer();
		trabalhar((*t).id);
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
        return "Penny";
    case 6:
        return "Stuart";
    case 7: 
        return "Kripke";  

    default:
        return NULL;
    }
}