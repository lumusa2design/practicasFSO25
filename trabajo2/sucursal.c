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
            printf("Soy el hijo (sucursal en %s, capacidad %s)\n", ciudad, capacidad);
            execlp("gnome-terminal", "gnome-terminal", "--wait", "--", "./minishell", ciudad, capacidad, (char *)NULL);
            perror("execlp");
            exit(EXIT_FAILURE);

        default:
            int status;
            waitpid(pid, &status, 0);
            printf("La sala \"%s\" ha cerrado.\n", ciudad);
            break;
    }
}

