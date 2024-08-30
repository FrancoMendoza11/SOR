#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

void* calculo_aritmetico(void* arg) {
    int contador = 0;
    while(contador < 2147483647) {
        contador = contador + 1;  
    }  
    return NULL;
}

int main() {
    pthread_t threads[5];

    for(int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, calculo_aritmetico, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

