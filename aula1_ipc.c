#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
   int fd[2];
   float medias[4] = {7.5, 8.0, 6.5, 9.0};
   char *meses[4] = {"Janeiro", "Fevereiro", "Março", "Abril"};
   if (pipe(fd) == -1) {
       perror("Erro ao criar pipe");
       return 1;
   }
   for (int i = 0; i < 4; i++) {
       pid_t pid = fork();
       if (pid < 0) {
           perror("Erro no fork");
           return 1;
       }
       if (pid == 0) {
           close(fd[0]);
           write(fd[1], &medias[i], sizeof(float));
           printf("Filho %s enviou a média %.2f\n", meses[i], medias[i]);
           close(fd[1]);
           exit(0);
       }
       wait(NULL);
   }
   close(fd[1]);
   printf("\nProcesso pai recebeu as médias:\n");
   for (int i = 0; i < 4; i++) {
       float mediaRecebida;
       read(fd[0], &mediaRecebida, sizeof(float));
       printf("%s: %.2f\n", meses[i], mediaRecebida);
   }
   close(fd[0]);
   return 0;
}

