#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void crea_sucursal(const char *ciudad, const char *capacidad)
{
    pid_t pid = fork();

    switch (pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:
            printf("Soy el hijo\n");
            execlp("gnome-terminal", "gnome-terminal", "--", "./minishell", ciudad, capacidad, (char *)NULL);
            perror("execlp");
            exit(EXIT_FAILURE);

        default:
            printf("Soy el padre\n");
            break;
    }
}

int main()
{
    crea_sucursal("hola", "5");
    return 0;
}

