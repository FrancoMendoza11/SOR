#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 


sem_t sem_asadoListo;
sem_t sem_invitadosSentados;
sem_t sem_manuchoSentado;
sem_t sem_comidaServidaManucho; 
sem_t sem_comidaServidaInvitados;
sem_t sem_platoTerminado;
sem_t sem_preguntaLanzada;
sem_t sem_respuestaLanzada;
sem_t sem_seLevantoManucho;
sem_t sem_platosDisponibles;
int cantSentados = 0;


void HacerAsado() {
    printf("Asador está preparando el asado...\n");
    sleep(2);  
}

void Sentarse() {
    printf("Sentándose...\n");
    sleep(1);
}

void Comer() {
    printf("Comiendo...\n");
    sleep(2);
}

void Lanzar_pregunta_mundialista() {
    printf("Manucho lanza una pregunta sobre el Mundial...\n");
    sleep(2);
}

void Lanzar_respuesta_mundialista() {
    printf("Un invitado responde la pregunta mundialista...\n");
    sleep(2);
}

void Enojarse() {
    printf("Manucho se enoja por la respuesta...\n");
    sleep(1);
}

void Levantarse() {
    printf("Levantándose...\n");
    sleep(1);
}

void ServirComida() {
    printf("Mozos están sirviendo la comida...\n");
    sleep(1);
}


void* asador(void* arg) {
    HacerAsado();  
    printf("Asado listo!\n");
    sem_post(&sem_asadoListo); 
    pthread_exit(NULL);
}


void* manucho(void* arg) {
    int N = *((int*) arg);
    sem_wait(&sem_invitadosSentados); 
    Sentarse();  
    printf("Manucho se sentó...\n");

	//se quito el for de aca ahora se hace en mozo
    sem_post(&sem_platosDisponibles); 

    sem_wait(&sem_comidaServidaManucho); 
    Comer();  
    printf("Manucho terminó de comer.\n");

    Lanzar_pregunta_mundialista();  
    sem_post(&sem_preguntaLanzada); 

    sem_wait(&sem_respuestaLanzada); 
    Enojarse();  
    Levantarse(); 
    sem_post(&sem_seLevantoManucho); 
    pthread_exit(NULL);
}


void* invitado(void* arg) {
    int N = *((int*) arg);
    Sentarse();  
    cantSentados++;

    if (cantSentados == N) {
        sem_post(&sem_invitadosSentados); // Señala que todos los invitados están sentados
    }

    sem_wait(&sem_comidaServidaInvitados); 
    Comer();  
    printf("Invitado terminó de comer...\n");
    sem_post(&sem_platoTerminado); 

    sem_wait(&sem_preguntaLanzada);  
    Lanzar_respuesta_mundialista();  
    sem_post(&sem_respuestaLanzada); 

    sem_wait(&sem_seLevantoManucho);  
    Levantarse();  
    pthread_exit(NULL);
}


void* mozo(void* arg) {
    int N = *((int*) arg);
    
    sem_wait(&sem_asadoListo); 
       
    sem_wait(&sem_platosDisponibles); 
    ServirComida();  // Sirve la comida a Manucho
    printf("Mozo sirvió comida a Manucho.\n");
    sem_post(&sem_comidaServidaManucho);  

    //se agrego este for para servir a todos los invitados
    for (int i = 0; i < N - 1; i++) {
        sem_post(&sem_platosDisponibles); // Señala que hay un plato disponible para cada invitado
        sem_wait(&sem_platosDisponibles); // Espera a que haya platos disponibles
        ServirComida();  // Sirve la comida a los invitados
        printf("Mozo sirvió comida a un invitado.\n");
        sem_post(&sem_comidaServidaInvitados);  
    }
    pthread_exit(NULL);
}


int main() {
    int N, M;

    printf("¿Cuántos invitados habrá?\n");
    scanf("%d", &N);

    // Determinar la cantidad de mozos
    M = N - 1;

  
    sem_init(&sem_asadoListo, 0, 0);
    sem_init(&sem_invitadosSentados, 0, 0);
    sem_init(&sem_manuchoSentado, 0, 0);
    sem_init(&sem_comidaServidaManucho, 0, 0); 
    sem_init(&sem_comidaServidaInvitados, 0, 0); 
    sem_init(&sem_platoTerminado, 0, 0);
    sem_init(&sem_preguntaLanzada, 0, 0);
    sem_init(&sem_respuestaLanzada, 0, 0);
    sem_init(&sem_seLevantoManucho, 0, 0);
    sem_init(&sem_platosDisponibles, 0, 0);

    pthread_t thread_asador, thread_manucho;
    pthread_t thread_invitados[N]; 
    pthread_t thread_mozos[M];     

    pthread_create(&thread_asador, NULL, asador, NULL);
    pthread_create(&thread_manucho, NULL, manucho, (void *) &N);

    for (int i = 0; i < N; i++) {
        pthread_create(&thread_invitados[i], NULL, invitado, (void *) &N);
    }

    for (int i = 0; i < M; i++) {
        pthread_create(&thread_mozos[i], NULL, mozo, (void *) &N);
    }

    pthread_join(thread_asador, NULL);
    pthread_join(thread_manucho, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(thread_invitados[i], NULL);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(thread_mozos[i], NULL);
    }

    sem_destroy(&sem_asadoListo);
    sem_destroy(&sem_invitadosSentados);
    sem_destroy(&sem_manuchoSentado);
    sem_destroy(&sem_comidaServidaManucho);
    sem_destroy(&sem_comidaServidaInvitados);
    sem_destroy(&sem_platoTerminado);
    sem_destroy(&sem_preguntaLanzada);
    sem_destroy(&sem_respuestaLanzada);
    sem_destroy(&sem_seLevantoManucho);
    sem_destroy(&sem_platosDisponibles);

    return 0;
}
