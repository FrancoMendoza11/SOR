#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int numero_secreto, adivinanza, intentos = 0;
    const int MAX_INTENTOS = 5;
    
    // semilla para generador de numeros aleatorios
    srand(time(NULL));
    
    numero_secreto = rand() % 100 + 1;
    
    printf("Bienvenido al juego de adivinanza de números!\n");
    printf("He pensado en un número entre 1 y 100. Tienes %d intentos para adivinarlo.\n", MAX_INTENTOS);
    
    while (intentos < MAX_INTENTOS) {
        printf("Ingresa tu adivinanza: ");
        scanf("%d", &adivinanza);
        intentos++;
        
        if (adivinanza < numero_secreto) {
            printf("Demasiado bajo. Intenta de nuevo.\n");
        } else if (adivinanza > numero_secreto) {
            printf("Demasiado alto. Intenta de nuevo.\n");
        } else {
            printf("Sos crack. Ganaste en %d intentos.\n", intentos);
            return 0;
        }
    }
    
    printf("Perdiste gil. El número secreto era %d.\n", numero_secreto);
    return 0;
}
