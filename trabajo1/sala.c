#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define ASIENTO_LIBRE 0

struct sala {
	char nombre[10];
	int capacidad;
	int asientos[];
};


int existe_sala();

struct sala *miSala = NULL;

int estado_asiento(int id_asiento) {
	if (!existe_sala()) {
		printf("La sala no existe.\n");
		return -1;
	}
	if (id_asiento > capacidad_sala() - 1 || id_asiento < 0) {
		printf("El asiento %i no existe.\n", id_asiento);
		return -1;
	}
	return miSala->asientos[id_asiento];
}

int asientos_libres() {
	if (!existe_sala()) {
		printf("La sala no existe.\n");
		return -1;
	}
	
	int count = 0;
	for (int i = 0; i < capacidad_sala(); i++) {
		if (estado_asiento(i) == ASIENTO_LIBRE) count++;
	}
	return count;
}

int asientos_ocupados () {
	if (!existe_sala()) {
		printf("La sala no existe.\n");
		return -1;
	}
	
	return capacidad_sala() - asientos_libres();
}

int capacidad_sala() {
	if (!existe_sala()) {
		printf("La sala no existe.\n");
		return -1;
	}
	return miSala->capacidad;
}

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
	for(int i = 0; i < capacidad; i++) {
		miSala->asientos[i] = ASIENTO_LIBRE;
	}
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

