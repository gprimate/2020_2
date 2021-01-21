#include <iostream>
#include <pthread.h>
#include <array>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string>

typedef struct {
	int id;
	int num_iteracoes;

} ThreadArg;

bool casal();

bool deadlock();

int getCasalId(int id);

int getOutroMembroCasal(int id);

void enfileirarCasal(int id);

void checkAndUpdateCasal(int id);

int next();

int timeDoingOtherThings();

void verifica();

void esperar(int id);

void esquentar(int id);

void liberar(int id);

void comer();

void *inicia_casais(void *arg);

void *inicia_raj(void *arg);

std::string getName(int id);

bool checkId(int id);