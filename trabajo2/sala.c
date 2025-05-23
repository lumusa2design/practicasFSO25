#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define ASIENTO_LIBRE 0

struct sala {
	char ciudad[20];
	int capacidad;
	int libres;
	int asientos[];
};


int existe_sala();
int capacidad_sala();
int estado_asiento(int id_asiento);

struct sala *miSala = NULL;

int libera_asiento(int id_asiento)
{
	if(!existe_sala())
	{
		fprintf(stderr, "La sala no existe\n");
		return -1;
	}
	
	if(id_asiento >= capacidad_sala() || id_asiento < 0 ) 
	{
		fprintf(stderr,"El asiento %d no existe\n", id_asiento);
		return -1;
	}
	if(estado_asiento(id_asiento) != ASIENTO_LIBRE) 
	{
		int estado_old = estado_asiento(id_asiento);
		miSala->libres ++;
		miSala->asientos[id_asiento] = ASIENTO_LIBRE;
		return estado_old;
	} else 
	{
		fprintf(stderr, "El asiento ya estaba vacío.\n");
		return -1;
	}	
}


int reserva_asiento(int id_persona)
{
	if(!existe_sala()) 
	{
		fprintf(stderr,"la sala no existe\n");
		return -1;
	} 
	if(id_persona < 0) 
	{
		fprintf(stderr, "El id de la persona tiene que ser positivo\n");
		return -1;
	}
	if(id_persona == ASIENTO_LIBRE)
	{
		fprintf(stderr, "El id introducido no es válido. Id introducido: %i\n", id_persona);
		return -1;
	}
	for(int i = 0; i <capacidad_sala(); i++)
	{
		if(estado_asiento(i) != -1 && estado_asiento(i) == ASIENTO_LIBRE) 
		{
			miSala->asientos[i] = id_persona;
			miSala->libres --;
			return i;
		}
	}
	fprintf(stderr, "La sala está llena.\n");
	return -1;
}


int estado_asiento(int id_asiento) {
	if (!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	if (id_asiento > capacidad_sala() - 1 || id_asiento < 0) {
		fprintf(stderr,"El asiento %i no existe.\n", id_asiento);
		return -1;
	}
	return miSala->asientos[id_asiento];
}


int asientos_libres() {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe.\n");
		return -1;
	}
	

	return miSala->libres;
}


int capacidad_sala() {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe.\n");
		return -1;
	}
	return miSala->capacidad;
}


int asientos_ocupados () {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe.\n");
		return -1;
	}
	
	return capacidad_sala() - asientos_libres();
}


int crea_sala(char nombre[20], int capacidad) {
	if (existe_sala()) {
		fprintf(stderr, "La sala ya está creada.\n");
		return -1;
	}
	
	if (capacidad <= 0) {
		fprintf(stderr, "La capacidad debe ser mayor que 0. Capacidad introducida: %i\n", capacidad);
		return -1;
	}
	
	miSala = (struct sala *) malloc(sizeof(struct sala) + capacidad * sizeof(int));
	if(miSala == NULL) 
	{
		fprintf(stderr,"Error al crear sala\n");
		return -1;
	}
	if (!existe_sala()) {
		fprintf(stderr, "Error de memoria.\n");
		return -1;
	}
	
	strcpy(miSala->ciudad, nombre);
	miSala->capacidad = capacidad;
	miSala->libres = capacidad;
	for(int i = 0; i < capacidad; i++) {
		miSala->asientos[i] = ASIENTO_LIBRE;
	}
	fprintf(stderr, "Sala creada correctamente.\n");
	return capacidad;
}


int elimina_sala() {
	if (!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	
	free(miSala);
	miSala = NULL;
	fprintf(stderr, "Sala eliminada correctamente.\n");
	return 0;
}


int existe_sala() {
	return miSala != NULL;
}


char * nombre_sala() {
	if (!existe_sala()) return NULL;
	return miSala->ciudad;
}
