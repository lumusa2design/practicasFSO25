#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sala.c"

int main(int argc, char * argv[]) {
	if(argc != 3) {
		fprintf(stderr, "Número incorrecto de parámetros.\nEsperado: 2\nRecibido: %i\n", argc-1);
		exit(-1);
	}
	
	if (crea_sala(argv[1], atoi(argv[2])) == -1) exit(-1);
	
	while (1) {
		char command[20];
		scanf("%s", &command);
		if (!strcmp(command, "salir")) {
			break;
		} else if (!strcmp(command, "ayuda")) {
			printf("Lista de comandos:\n");
			printf("(por implementar)\n"); // TODO: Mostrar lista de comandos
		} else if(!strcmp(command, "reserva"))
		{
			int id;
			printf("Introduzca el id de la persona: ");
			scanf("%d", &id);
			reserva_asiento(id);
		} else {
			fprintf(stderr, "Comando desconocido.\nEscribe \"ayuda\" para ver la lista de comandos.\n");
		}
	}
	elimina_sala();
	exit(0);
}
