#include <iostream>
#include <unistd.h>
#include <pthread.h>

void * myturn(void * arg) {
   for(int i = 0; i < 10; i++) {
      sleep(1);
      std::cout << "my turn " << i << std::endl;
   }
   return NULL;
}

void yourturn() {
   for(int i = 0; i < 5; i++) {
      sleep(2);
      std::cout << "your turn " << i << std::endl;

   }
}


int main(int argc, char const *argv[]) {
   pthread_t kiriThread;
   pthread_create(&kiriThread, NULL, myturn, NULL);


   yourturn();

   //makes sure the thread will run until it ends
   pthread_join(kiriThread, NULL);
   
   return 0;
}
