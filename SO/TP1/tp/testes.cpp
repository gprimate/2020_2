#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>


int timeDoingOtherThings () {
    double time = drand48();
    time = time * 4 + 3;
    return (int) time;
}

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 200; i++) {
        
        std::cout << timeDoingOtherThings() << std::endl;

    }
    return 0;
}
