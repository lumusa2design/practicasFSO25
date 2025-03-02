#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct sala {
	char nombre[10];
	int capacidad;
	int asientos[];
};

int existe_sala();

struct sala *miSala = NULL;

int crea_sala(int capacidad) {
	if (existe_sala()) {
		printf("La sala ya está creada.\n");
		return -1;
	}
	
	miSala = malloc(sizeof(struct sala) + capacidad * sizeof(int));
	if (!existe_sala()) {
		printf("Error de memoria.\n");
		return -1;
	}
	
	strcpy(miSala->nombre, "");
	miSala->capacidad = capacidad;
	printf("Sala creada correctamente.\n");
	return capacidad;
}

int elimina_sala() {
	if (!existe_sala()) {
		printf("La sala no existe.\n");
		return -1;
	}
	
	free(miSala);
	miSala = NULL;
	printf("Sala eliminada correctamente.\n");
	return 0;
}

int existe_sala() {
	return miSala != NULL;
}
