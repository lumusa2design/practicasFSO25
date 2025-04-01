#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sala.c"

void print_help();

int main(int argc, char * argv[]) {
	if(argc != 3) {
		fprintf(stderr, "Número incorrecto de parámetros.\nEsperado: 2\nRecibido: %i\n", argc-1);
		exit(-1);
	}
	
	if (crea_sala(argv[1], atoi(argv[2])) == -1) exit(-1);
	
	while (1) {
		char command[20];
		// scanf("%s", &command);
		fgets(command, sizeof(command), stdin);
		command[strcspn(command, "\n")] = 0;
		char *token = strtok(command, " ");
		
		// TODO: Implementar todos los comandos.
		if (!strcmp(command, "cerrar_sala")) {
			break;
		} else if (!strcmp(command, "ayuda")) {
			print_help();
		} else if(!strcmp(token, "reserva ")) {
		} else if (!strcmp(token, "libera")) {
		} else if (!strcmp(token, "estado_asiento")) {
		} else if (token == NULL) {
		} else if (!strcmp(command, "estado_sala")) {
		} else {
			fprintf(stderr, "Comando '%s' desconocido.\nEscribe \"ayuda\" para ver la lista de comandos.\n", token);
		}
	}
	elimina_sala();
	exit(0);
}

int strsplit(char* str, char sep, char* dst[]) { }

void print_help() {
	printf("Lista de comandos:\n");
	printf("reserva X - Realiza una reserva para la persona con id X.\n");
	printf("libera X - Libera el asiento de la persona X.\n");
	printf("estado_asiento X - Muestra el estado del asiento X.\n");
	printf("estado_sala - Muestra el estado de la sala.\n");
	printf("cerrar_sala - Cierra la sala.\n");
	printf("ayuda - Muestra la lista de comandos.\n");
}
