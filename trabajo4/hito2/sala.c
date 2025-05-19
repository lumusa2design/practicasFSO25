#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "sala.h"
#include "retardo.h"

#define ASIENTO_LIBRE 0

struct sala {
    char nombre[10];
    int capacidad;
    int libres;
    int asientos[];
};

struct sala *miSala = NULL;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

int existe_sala();
int digitos_asiento();

int libera_asiento(int id_asiento) {
    pthread_mutex_lock(&mymutex);

    if (!existe_sala() || id_asiento < 0 || id_asiento >= miSala->capacidad) {
        pthread_mutex_unlock(&mymutex);
        return -1;
    }

    if (miSala->asientos[id_asiento] > 0) {
    	pausa_aleatoria(MAX_PAUSA);
        int estado_old = miSala->asientos[id_asiento];
        miSala->asientos[id_asiento] = ASIENTO_LIBRE;
        miSala->libres++;

	pausa_aleatoria(MAX_PAUSA);
        pthread_mutex_unlock(&mymutex);
        return estado_old;
    }

    pthread_mutex_unlock(&mymutex);
    return -1;
}

int reserva_asiento(int id_persona) {
    pthread_mutex_lock(&mymutex);

    if (!existe_sala() || id_persona <= 0) {
        pthread_mutex_unlock(&mymutex);
        return -1;
    }
    

    for (int i = 0; i < miSala->capacidad; i++) {
        if (miSala->asientos[i] == ASIENTO_LIBRE) {
            pausa_aleatoria(MAX_PAUSA);
            miSala->asientos[i] = id_persona;
            miSala->libres--;

	    
            pthread_mutex_unlock(&mymutex);
            return i;
        }
    }

    pthread_mutex_unlock(&mymutex);
    return -1;
}

int estado_asiento(int id_asiento) {
    if (!existe_sala() || id_asiento < 0 || id_asiento >= miSala->capacidad)
        return -1;
    return miSala->asientos[id_asiento];
}

int asientos_libres() {
    return existe_sala() ? miSala->libres : -1;
}

int capacidad_sala() {
    return existe_sala() ? miSala->capacidad : -1;
}

int asientos_ocupados() {
    return existe_sala() ? miSala->capacidad - miSala->libres : -1;
}

int crea_sala(int capacidad) {
    if (capacidad <= 0 || existe_sala())
        return -1;

    miSala = malloc(sizeof(struct sala) + capacidad * sizeof(int));
    if (!miSala) return -1;

    strcpy(miSala->nombre, "");
    miSala->capacidad = capacidad;
    miSala->libres = capacidad;

    for (int i = 0; i < capacidad; i++)
        miSala->asientos[i] = ASIENTO_LIBRE;

    return capacidad;
}

int elimina_sala() {
    pthread_mutex_lock(&mymutex);
    free(miSala);
    miSala = NULL;
    pthread_mutex_unlock(&mymutex);
    return 0;
}

int existe_sala() {
    return miSala != NULL;
}

void estado_sala() {
    pthread_mutex_lock(&mymutex);
    printf("Capacidad: %i\n", capacidad_sala());
    printf("Libres: %d | Ocupados: %d\n", asientos_libres(), asientos_ocupados());
    for (int i = 0; i < capacidad_sala(); i++) {
        printf("[%*i] %-8i", digitos_asiento(), i, estado_asiento(i));
        if ((i + 1) % 5 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
    printf("\n");
    pthread_mutex_unlock(&mymutex);
}

int digitos_asiento() {
    int digitos = 0;
    for (int capacidad = capacidad_sala(); capacidad > 0; capacidad /= 10)
        digitos++;
    return digitos;
}

