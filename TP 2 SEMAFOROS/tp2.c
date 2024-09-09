#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para usar sleep()

// Declaración de semáforos
sem_t sem_asadoListo;
sem_t sem_invitadosSentados;
sem_t sem_manuchoSentado;
sem_t sem_comidaServida;
sem_t sem_manuchoTerminaDeComer;
sem_t sem_platoTerminado;
sem_t sem_preguntaLanzada;
sem_t sem_respuestaLanzada;
sem_t sem_seLevantoManucho;
sem_t sem_platosDisponibles;
int cantSentados = 0;

void HacerAsado() {
    printf("Asador está preparando el asado...\n");
//    sleep(5);  // Simula el tiempo para hacer el asado
}

void Sentarse() {
    printf("Sentándose...\n");
//    sleep(5);
}

void Comer() {
    printf("Comiendo...\n");
//    sleep(5);
}

void Lanzar_pregunta_mundialista() {
    printf("Manucho lanza una pregunta sobre el Mundial...\n");
//    sleep(5);
}

void Lanzar_respuesta_mundialista() {
    printf("Un invitado responde la pregunta mundialista...\n");
 //   sleep(5);
}

void Enojarse() {
    printf("Manucho se enoja por la respuesta...\n");
//    sleep(5);
}

void Levantarse() {
    printf("Levantándose...\n");
//    sleep(5);
}

void ServirComida() {
    printf("Mozos están sirviendo la comida...\n");
//    sleep(5);
}

// Función del Asador
void* asador(void* arg) {
    HacerAsado();  // El asador prepara el asado
    printf("Asado listo!\n");
    sem_post(&sem_asadoListo); // Señala que el asado está listo
    pthread_exit(NULL);
}

// Función de Manucho
void* manucho(void* arg) {
	int N = *((int*) arg);
    sem_wait(&sem_invitadosSentados); // Espera a que todos los invitados estén sentados
    Sentarse();  // Manucho se sienta
	for (int i = 0; i < N; i++) {
        sem_post(&sem_platosDisponibles); // Señala que Manucho se sentó
    }
	printf("se sento manucho... \n");
	
	for (int i = 0; i < N; i++) {
        sem_post(&sem_manuchoSentado); // Señala que Manucho se sentó
    }
	
    sem_wait(&sem_comidaServida); // Espera a que le sirvan la comida
    Comer();  // Manucho come
	printf("manucho termino de comer \n");
//    sem_post(&sem_manuchoTerminaDeComer); // Señala que terminó de comer

    Lanzar_pregunta_mundialista();  // Lanza la pregunta sobre el Mundial
    sem_post(&sem_preguntaLanzada); // Señala que la pregunta fue lanzada

    sem_wait(&sem_respuestaLanzada); // Espera una respuesta de los invitados
    Enojarse();  // Manucho se enoja por la respuesta
    Levantarse();  // Manucho se levanta
    sem_post(&sem_seLevantoManucho); // Señala que Manucho se levantó
    pthread_exit(NULL);
}

// Función de los Invitados
void* invitado(void* arg) {
	int N = *((int*) arg);
    Sentarse();  // El invitado se sienta
    cantSentados++;
	printf("%d",cantSentados);
	if (cantSentados==N){
		sem_post(&sem_invitadosSentados); // Señala que todos sentados
	}	
    sem_wait(&sem_comidaServida);  // Espera a que le sirvan la comida
    Comer();  // Come la comida
	printf("invitado termina de comer... \n");
    sem_post(&sem_platoTerminado); // Señala que terminó de comer

    sem_wait(&sem_preguntaLanzada);  // Espera a que Manucho haga la pregunta
    Lanzar_respuesta_mundialista();  // Responde la pregunta sobre el Mundial
    sem_post(&sem_respuestaLanzada); // Señala que respondió

    sem_wait(&sem_seLevantoManucho);  // Espera a que Manucho se levante
    Levantarse();  // Se levanta
    pthread_exit(NULL);
}

// Función de los Mozos
void* mozo(void* arg) {
    sem_wait(&sem_asadoListo); // Espera a que el asado esté listo
//    sem_wait(&sem_manuchoSentado); // Espera a que Manucho esté sentado
	sem_wait(&sem_platosDisponibles);
	
    ServirComida();  // Sirve la comida
	
    sem_post(&sem_comidaServida);  // Señala que se ha servido comida
	printf("sirvio comida \n");
    pthread_exit(NULL);
}

// Función principal (Main)
int main() {
    int N, M;

    // Pedir el número de invitados
    printf("¿Cuántos invitados habrá?\n");
    scanf("%d", &N);

    // Determinar la cantidad de mozos
    M = N - 1;

    // Inicialización de semáforos
    sem_init(&sem_asadoListo, 1, 0);
    sem_init(&sem_invitadosSentados, 1, 0);
    sem_init(&sem_manuchoSentado, 1, 0);
    sem_init(&sem_comidaServida, 1, 0);
    sem_init(&sem_manuchoTerminaDeComer, 1, 0);
    sem_init(&sem_platoTerminado, 1, 0);
    sem_init(&sem_preguntaLanzada, 1, 0);
    sem_init(&sem_respuestaLanzada, 1, 0);
    sem_init(&sem_seLevantoManucho, 1, 0);
	sem_init(&sem_platosDisponibles, 1, 0);

    pthread_t thread_asador, thread_manucho;
    pthread_t thread_invitados[N]; // N es el número de invitados
    pthread_t thread_mozos[M];     // M es el número de mozos (invitados - 1)

    // Creación de threads
    pthread_create(&thread_asador, NULL, asador, NULL);
    pthread_create(&thread_manucho, NULL, manucho, (void *) &N);

    for (int i = 0; i < N; i++) {
        pthread_create(&thread_invitados[i], NULL, invitado, (void *) &N);
    }
	

    for (int i = 0; i < M; i++) {
        pthread_create(&thread_mozos[i], NULL, mozo, NULL);
    }

    // Espera a que todos los threads terminen
    pthread_join(thread_asador, NULL);
    pthread_join(thread_manucho, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(thread_invitados[i], NULL);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(thread_mozos[i], NULL);
    }

    // Destrucción de semáforos
    sem_destroy(&sem_asadoListo);
    sem_destroy(&sem_invitadosSentados);
    sem_destroy(&sem_manuchoSentado);
    sem_destroy(&sem_comidaServida);
    sem_destroy(&sem_manuchoTerminaDeComer);
    sem_destroy(&sem_platoTerminado);
    sem_destroy(&sem_preguntaLanzada);
    sem_destroy(&sem_respuestaLanzada);
    sem_destroy(&sem_seLevantoManucho);
	sem_destroy(&sem_platosDisponibles);

    return 0;
}

