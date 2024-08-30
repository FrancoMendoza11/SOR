#include <stdio.h>
#include <unistd.h>

int calculoRandom(int x) {
    return x + 10000 * 50;
}

int main() {
    int a, b, resultado;
    
    printf("PID del proceso: %d\n", getpid());

    printf("Introduce el primer número: ");
    scanf("%d", &a);
    calculoRandom(a);
    
    printf("Introduce el segundo número: ");
    scanf("%d", &b);
    calculoRandom(b);

    return 0;
}

