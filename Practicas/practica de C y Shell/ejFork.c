#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid;
  pid = fork ();
  printf("Chau\n");

  if (pid < 0) { 
    fprintf(stderr, "Fork ha fallado");
    return 1;
  }
  else if (pid == 0) { /* proceso hijo */
    printf("Soy el proceso Hijo\n");
    execlp("/bin/ls","ls",NULL);
  } 
  else { /* proceso padre */
    printf("Esperando al proceso Hijo\n");
    wait(NULL);
    printf("El proceso Hijo ha terminado\n");
  }
}

