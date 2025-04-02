#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sala.h"

void print_help();
void print_estado_sala();
int digitos_asiento();


int main(int argc, char * argv[]) {
	if(argc != 3) {
		fprintf(stderr, "Número incorrecto de parámetros.\nEsperado: 2\nRecibido: %i\n", argc-1);
		exit(-1);
	}
	
	if (crea_sala(argv[1], atoi(argv[2])) == -1) exit(-1);
	
	while (1) {
		char command[20];
		// scanf(": %s", &command);
		printf(": ");
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = 0;
		char *token = strtok(command, " ");
		
		// TODO: Implementar todos los comandos.
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
			libera_asiento((atoi(token)));
			
		} else if (!strcmp(token, "estado_asiento")) {
			
		} else if (!strcmp(command, "estado_sala")) {
			print_estado_sala();
			
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
	printf("reserva X - Realiza una reserva para la persona con id X.\n");
	printf("libera X - Libera el asiento de la persona X.\n");
	printf("estado_asiento X - Muestra el estado del asiento X.\n");
	printf("estado_sala - Muestra el estado de la sala.\n");
	printf("cerrar_sala - Cierra la sala.\n");
	printf("ayuda - Muestra la lista de comandos.\n\n");
}
