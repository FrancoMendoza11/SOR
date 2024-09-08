#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 10 // Número de invitados
#define M 5  // Número de mozos

sem_t sem_servir;
sem_t sem_comer;
sem_t sem_manucho;

// Función para el proceso de mozo
void* mozo(void* arg) {
    // Código para servir comida
    sem_wait(&sem_servir);
    // Servir comida
    sem_post(&sem_servir);
    pthread_exit(NULL);
}

// Función para el proceso de Manucho
void* manucho(void* arg) {
    // Esperar a que todos los invitados se hayan sentado
    // y la comida haya sido servida
    sem_wait(&sem_manucho);
    // Lanzar la pregunta mundialista
    sem_post(&sem_manucho);
    pthread_exit(NULL);
}

// Función para el proceso de comensal
void* comensal(void* arg) {
    // Esperar a ser servido
    sem_wait(&sem_servir);
    // Comer
    sem_post(&sem_comer);
    // Esperar la pregunta
    sem_wait(&sem_manucho);
    // Responder la pregunta
    sem_post(&sem_manucho);
    // Esperar que Manucho se enoje
    sem_wait(&sem_comer);
    // Levantarse si ha terminado de comer
    sem_post(&sem_comer);
    pthread_exit(NULL);
}

int main() {
    sem_init(&sem_servir, 0, N+1);
    sem_init(&sem_comer, 0, 1);
    sem_init(&sem_manucho, 0, 0);

    pthread_t mozos[M];
    pthread_t manucho_thread;
    pthread_t comensales[N];

    // Crear procesos para mozos
    for (int i = 0; i < M; i++) {
        pthread_create(&mozos[i], NULL, mozo, NULL);
    }

    // Crear proceso para Manucho
    pthread_create(&manucho_thread, NULL, manucho, NULL);

    // Crear procesos para comensales
    for (int i = 0; i < N; i++) {
        pthread_create(&comensales[i], NULL, comensal, NULL);
    }

    // Esperar que terminen los procesos
    for (int i = 0; i < M; i++) {
        pthread_join(mozos[i], NULL);
    }
    pthread_join(manucho_thread, NULL);
    for (int i = 0; i < N; i++) {
        pthread_join(comensales[i], NULL);
    }

    sem_destroy(&sem_servir);
    sem_destroy(&sem_comer);
    sem_destroy(&sem_manucho);

    return 0;
}
