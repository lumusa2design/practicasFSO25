#include <stdio.h>
#include <string.h>
#include "sucursal.h"

int main(int argc, char argv[]) {
	char ciudad[20];
	char capacidad[3]; // Max capacidad = 999
	while (true) {
		printf("Ciudad:");
		scanf ("%S", &ciudad);
		if (!strcmp(ciudad, "salir")) break;
		printf("Capacidad:");
		scanf ("%S", &capacidad);
		
		crea_sucursal(ciudad, capacidad);
	}
}

