#include <stdio.h>
#include <string.h>
#include <signal.h>	
#include "sucursal.h"

int main(int argc, char argv[]) {
	signal (SIGCHLD, manejador_sala_terminada);
	
	while (1) {
		char ciudad[20] = "";
		char capacidad[3] = ""; // Max capacidad = 999
		
		fflush(stdin);
		while(!strcmp(ciudad, "")) {
			printf("\nCiudad: ");
			fgets (ciudad, sizeof(ciudad), stdin);
			ciudad[strcspn(ciudad, "\n")] = 0;
		}
		if (!strcmp(ciudad, "salir")) break;
		
		fflush(stdin);
		while(!strcmp(capacidad, "")) {
			printf("\nCapacidad: ");
			fgets (capacidad, sizeof(capacidad), stdin);
			capacidad[strcspn(capacidad, "\n")] = 0;
		}
		
		crea_sucursal(ciudad, capacidad);
		printf("\n");
	}
}

