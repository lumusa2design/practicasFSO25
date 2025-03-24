#include <stdio.h>
#include "sala.h"
#include "test_sala.h"

int main(int argc, char argv[]) {
	while (true) {
		char nombre[10];
		char ciudad[20];
		int capacidad;
		
		printf("Nombre de la sala:");
		scanf ("%S", &nombre);
		
		printf("Ciudad:");
		scanf ("%S", &ciudad);
		printf("Capacidad:");
		scanf ("%i", &capacidad);
	}
}

