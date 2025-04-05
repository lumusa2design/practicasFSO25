#include <stdio.h>
#include <string.h>
#include <signal.h>	
#include "sucursal.h"

int main(int argc, char argv[]) {
	char ciudad[20] = "";
	char capacidad[3] = ""; // Max capacidad = 999
	while (1) {
		signal (SIGCHLD, manejador_sala_terminada);
		
		printf("Ciudad: ");
		scanf ("%s", &ciudad);
		if (!strcmp(ciudad, "salir")) break;
		printf("Capacidad: ");
		scanf ("%s", &capacidad);
		
		crea_sucursal(ciudad, capacidad);
		printf("\n");
	}
	signal (SIGCHLD, manejador_sala_terminada);
}

