#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>


int timeDoingOtherThings();

std::string getName(int id);

void * bananinha(void * pessego);

void eat(int id);

void warmUpSomething(int id);

void wait(int id);

bool checkPartner(int id);

typedef struct {
	int id;
	int numIter;

} ThreadInfo;