#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct sala {
	char nombre[10];
	int capacidad;
	int asientos[];
};

struct sala *miSala = NULL;

int crea_sala(int capacidad) {
	if (miSala != NULL) {
		printf("La sala ya está creada.\n");
		return -1;
	}
	
	miSala = malloc(sizeof(struct sala) + capacidad * sizeof(int));
	if (miSala == NULL) {
		printf("Error de memoria.\n");
		return -1;
	}
	
	strcpy(miSala->nombre, "");
	miSala->capacidad = capacidad;
	printf("Sala creada correctamente.\n");
	return capacidad;
}
