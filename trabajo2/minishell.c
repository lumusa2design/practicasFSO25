#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sala.h"

void print_help();
void print_estado_sala();
int digitos_asiento();
void clear_command();

int main(int argc, char * argv[]) {
	if(argc != 3) {
		fprintf(stderr, "Número incorrecto de parámetros.\nEsperado: 2\nRecibido: %i\n", argc-1);
		exit(-1);
	}
	
	if (crea_sala(argv[1], atoi(argv[2])) == -1) exit(-1);
	
	while (1) {
		char command[20];

		printf(": ");
		
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = 0;
		if (!strcmp(command, "")) continue;
		
		char *token = strtok(command, " ");
		
		if (!strcmp(command, "cerrar_sala")) {
			break;
			
		} else if (!strcmp(command, "ayuda")) {
			print_help();
			
		} else if(!strcmp(token, "reserva")) {
			token = strtok(NULL, " ");
			if (token == NULL) {
				fprintf(stderr, "Uso: reserva X");
				continue;
			}
			reserva_asiento(atoi(token));
			
		} else if (!strcmp(token, "libera")) {
			token = strtok(NULL, " ");
			if (token == NULL) {
				fprintf(stderr, "Uso: libera X");
				continue;
			}
			libera_asiento(atoi(token));
			
		} else if (!strcmp(token, "estado_asiento")) {
			token = strtok(NULL, " ");
			if (token == NULL) {
				fprintf(stderr, "Uso: estado_asiento X");
				continue;
			}
			int id = atoi(token);
			printf("Asiento %i: %i\n", id, estado_asiento(id));
			
		} else if (!strcmp(command, "estado_sala")) {
			print_estado_sala();
			
		} else if (!strcmp(command, "clear")) {
			clear_command();
			
		} else {
			fprintf(stderr, "Comando '%s' desconocido.\nEscribe \"ayuda\" para ver la lista de comandos.\n", token);
		}
	}
	elimina_sala();
	exit(0);
}


void print_estado_sala() {
	printf("Sala: %s\n", nombre_sala());
	printf("Capacidad: %i\n", capacidad_sala());
	printf("Asientos libres: %i\n", asientos_libres());
	printf("Asientos ocupados: %i\n", asientos_ocupados());
	for (int i=0; i < capacidad_sala(); i++) {
		printf("[%*i] %-8i%s", digitos_asiento(), i, estado_asiento(i), (i+1)%5==0? "\n" : "");
	}
	printf("\n");
}


int digitos_asiento() {
	int digitos = 0;
	for (int capacidad = capacidad_sala(); capacidad > 0; capacidad /=10) digitos++;
	return digitos;
}


void print_help() {
	printf("Lista de comandos:\n");
	printf("%-16s - Realiza una reserva para la persona con id X.\n", "reserva X");
	printf("%-16s - Libera el asiento de la persona X.\n", "libera X");
	printf("%-16s - Muestra el estado del asiento X.\n", "estado_asiento X");
	printf("%-16s - Muestra el estado de la sala.\n", "estado_sala");
	printf("%-16s - Cierra la sala.\n", "cerrar_sala");
	printf("%-16s - Vacía la pantalla.\n", "clear");
	printf("%-16s - Muestra la lista de comandos.\n\n", "ayuda");
}


void clear_command() {
	pid_t pid = fork();
	switch(pid) {
		case -1:
			// Error
			fprintf(stderr, "Error en el fork.\n");
			break;
		case 0:
			// Hijo
			execlp("clear", "clear", NULL);
			fprintf(stderr, "No se pudo ejecutar el comando clear.\n");
			break;
		default:
			int status;
			waitpid(pid, &status, 0);
			break;
	}
}
