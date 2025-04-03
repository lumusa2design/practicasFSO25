#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void crea_sucursal(const char *ciudad, const char *capacidad)
{
    pid_t pid = fork();

    switch (pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:
            // printf("Soy el hijo\n");
            execlp("gnome-terminal", "gnome-terminal", "--", "./minishell", ciudad, capacidad, (char *)NULL);
            perror("execlp");
            exit(EXIT_FAILURE);

        default:
            // printf("Soy el padre\n");
            int status;
            waitpid(pid, &status, 0); // FIXME: No espera.
            printf("La sala \"%s\" ha cerrado.\n", ciudad);
            break;
    }
}

/*int main(int argc, char * argv[])
{
	if(argc != 3) {
		fprintf(stderr, "Número incorrecto de parámetros.\nEsperado: 2\nRecibido: %i\n", argc-1);
		exit(-1);
	}
    crea_sucursal(argv[1], argv[2]);
    return 0;
}*/

