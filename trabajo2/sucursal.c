#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SALAS 100

struct SalaInfo {
    pid_t pid;
    char nombre[50];
};

struct SalaInfo salas[MAX_SALAS];
int num_salas = 0;

void manejador_sala_terminada(int sig) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < num_salas; i++) {
            if (salas[i].pid == pid) {
                printf("\n[INFO] La sala \"%s\" (PID %d) ha finalizado.\n", salas[i].nombre, pid);
                if (WIFEXITED(status)) {
                    int exit_code = WEXITSTATUS(status);
                    if (exit_code == 0) {
                        printf("La sala cerró con todos los asientos ocupados.\n");
                    } else if (exit_code == 1) {
                        printf("La sala cerró con algunos asientos libres.\n");
                    } else {
                        printf("Nombre de salida de salida de sala inesperado: %d\n", exit_code);
                    }
                } else {
                    printf("La sala no terminó normalmente.\n");
                }
                break;
            }
        }
    }
}

void crea_sucursal(const char *ciudad, const char *capacidad) {
    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:
            // printf("Soy el hijo\n");
            execlp("gnome-terminal", "gnome-terminal", "--wait", "--", "./minishell", ciudad, capacidad, (char *)NULL);
            perror("execlp");
            exit(EXIT_FAILURE);

        default:
            // printf("Soy el padre\n");
            int status;
            // waitpid(pid, &status, 0);
            // printf("La sala \"%s\" ha cerrado.\n", ciudad);
            if (num_salas < MAX_SALAS) {
                salas[num_salas].pid = pid;
                strncpy(salas[num_salas].nombre, ciudad, sizeof(salas[num_salas].nombre));
                num_salas++;
            }
            break;
    }
}

/* int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ciudad> <capacidad>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *ciudad = argv[1];
    const char *capacidad = argv[2];

    signal(SIGCHLD, manejador_sala_terminada);

    crea_sucursal(ciudad, capacidad);

    while (1) {
        pause();
    }

    return 0;
}*/

