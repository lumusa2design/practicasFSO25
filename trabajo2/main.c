#include <stdio.h>
#include <string.h>
#include "sucursal.h"

int main(int argc, char argv[]) {
	char ciudad[20];
	int capacidad;
	while (true) {
		printf("Ciudad:");
		scanf ("%S", &ciudad);
		if (!strcmp(ciudad, "salir")) break;
		printf("Capacidad:");
		scanf ("%i", &capacidad);
		
		crea_sucursal(ciudad, capacidad);
	}
}

