#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sala.h"
#include "retardo.h"

# define ASIENTO_LIBRE 0

struct sala {
	char nombre[10];
	int capacidad;
	int libres;
	int asientos[];
};


int existe_sala();
int digitos_asiento();

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
		fprintf(stderr,"el asiento %d no existe\n", id_asiento);
		return -1;
	}
	if(estado_asiento(id_asiento) > 0) 
	{
		pausa_aleatoria(MAX_PAUSA);
		int estado_old = estado_asiento(id_asiento);
		miSala->libres ++;
		miSala->asientos[id_asiento] = ASIENTO_LIBRE;
		pausa_aleatoria(MAX_PAUSA);
		return estado_old;
	} else 
	{
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
	if(id_persona == ASIENTO_LIBRE) {
		fprintf(stderr, "Id incorrecto. Id introducido: %d\n", id_persona);
		return -1;
	}
	for(int i = 0; i <capacidad_sala(); i++)
	{
		if(estado_asiento(i) != -1 && estado_asiento(i) == ASIENTO_LIBRE) 
		{
			pausa_aleatoria(MAX_PAUSA);
			miSala->asientos[i] = id_persona;
			miSala->libres --;
			pausa_aleatoria(MAX_PAUSA);
			return i;
		}
	}
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


int crea_sala(int capacidad) {
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
	
	strcpy(miSala->nombre, "");
	miSala->capacidad = capacidad;
	miSala->libres = capacidad;
	for(int i = 0; i < capacidad; i++) {
		miSala->asientos[i] = ASIENTO_LIBRE;
	}
	return capacidad;
}

int elimina_sala() {
	if (!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	
	free(miSala);
	miSala = NULL;
	return 0;
}

int existe_sala() {
	return miSala != NULL;
}

void estado_sala() {
    pausa_aleatoria(MAX_PAUSA);
    printf("Sala: %s\n", miSala->nombre);
    printf("Capacidad: %i\n", capacidad_sala());
    printf("Libres: %d | Ocupados: %d\n", asientos_libres(), asientos_ocupados());
    for (int i = 0; i < capacidad_sala(); i++) {
        printf("[%*i] %-8i%s", digitos_asiento(), i, estado_asiento(i), (i+1)%5==0? "\n" : "");
    }
    printf("\n");
}

int digitos_asiento() {
    int digitos = 0;
    for (int capacidad = capacidad_sala(); capacidad > 0; capacidad /=10) digitos++;
    return digitos;
}

