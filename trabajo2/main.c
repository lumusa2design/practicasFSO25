#include <stdio.h>
#include "sala.h"
#include "test_sala.h"

int main(int argc, char argv[]) {
	char ciudad[20];
	int capacidad;
	while (true) {
		printf("Ciudad:");
		scanf ("%S", &ciudad);
		printf("Capacidad:");
		scanf ("%i", &capacidad);
		
		crea_sucursal(ciudad, capacidad);
	}
}

