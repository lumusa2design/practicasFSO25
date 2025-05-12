#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "sala.h"
#include "retardo.h"

#define ITERACIONES_HILO 3

void * hilo_reservas(void *args) {
	int asientos[ITERACIONES_HILO];
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		asiento[i] = reserva_asiento(i+20);
		pausa_aleatoria(1.5);
	}
	
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		libera_asiento(asientos[i]);
	}
}

void * hilo_estado(void *args) {
	// TODO
}


int main(int argc, char argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Uso: ./ numero_de_hilos");
	}
	
	int n_threads = atoi(argv[1]);
	
	pthread_t threads;
	if ((threads=malloc(n_threads*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos: %s", strerror(errno));
	}
	
	
}

