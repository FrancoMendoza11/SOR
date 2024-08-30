#include <stdio.h>  //incluimos la libreria de estandar input/output
#include <unistd.h> //para hacer sleep
#include <time.h>   //para inicializar el tiempo

void calculo_aritmetico(){
    int contador=0;
    while(contador < 2147483647){
  	contador=contador+1;  
    }  
}

int main() {
  calculo_aritmetico(); 
  calculo_aritmetico(); 
  calculo_aritmetico(); 
  calculo_aritmetico(); 
  calculo_aritmetico(); 
  return 0;
}

