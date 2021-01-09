#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void * myturn(void * arg) {
   for(int i = 0; i < 10; i++) {
      sleep(1);
      printf("my turn! %d\n", i);
   }
   return NULL;
}

void yourturn() {
   for(int i = 0; i < 5; i++) {
      sleep(2);
      printf("your turn! %d\n", i);

   }
}


int main(int argc, char const *argv[]) {
   pthread_t kiriThread;
   pthread_create(&kiriThread, NULL, myturn, NULL);


   yourturn();

   pthread_join(&kiriThread, NULL);
   return 0;
}
