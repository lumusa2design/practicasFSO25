#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "sala.h"

#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_RESET "\033[0m"

#define ITERACIONES_HILO 3
#define CAPACIDAD_SALA 20

void* hilo_reservas(void* arg) {
	
	int id_hilo = *(int*)arg;
	int base = id_hilo*10;
	
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		printf("%sHilo reservas %d:%s Reservando asiento para id %d...\n", COLOR_YELLOW, id_hilo, COLOR_RESET, i+base);
		int resultado = reserva_asiento(i+base);
		if (resultado != -1) {
			printf("%sHilo reservas %d:%s Asiento %d reservado para id %d.\n", COLOR_YELLOW, id_hilo, COLOR_RESET, resultado, i+base);
		} else {
			printf("%sHilo reservas %d:%s ERROR: No se ha podido reservar el asiento para la id %d.\n%s", COLOR_YELLOW, id_hilo, COLOR_RED, i+base, COLOR_RESET);
		}
	}
	
	printf("%sHilo reservas %d finalizado.\n%s", COLOR_YELLOW, id_hilo, COLOR_RESET);
}

void* hilo_liberacion(void*arg) {
	int id_hilo = *(int*)arg;
	
	for (int i=0; i < ITERACIONES_HILO; i++) {
		printf("%sHilo liberaciones %d:%s Liberando asiento...\n", COLOR_YELLOW, id_hilo, COLOR_RESET);
		int asiento = libera_asiento();
		if (asiento == -1) {
			printf("%sHilo liberaciones %d:%s ERROR: No se ha podido liberar un asiento.\n%s", COLOR_YELLOW, id_hilo, COLOR_RED, COLOR_RESET);
		} else {
			printf("%sHilo liberaciones %d:%s Asiento %d liberado.\n", COLOR_YELLOW, id_hilo, COLOR_RESET, asiento);
		}
	}
	
	printf("%sHilo liberaciones %d finalizado.\n%s", COLOR_YELLOW, id_hilo, COLOR_RESET);
}

void* hilo_estado(void* args) {
	while(1) {
		estado_sala();
	}
}


int ret;
int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Uso: ./ n_hilos_reservas n_hilos_liberaciones\n");
		exit(-1);
	}
	
	
	int n_reservations = atoi(argv[1]);
	int n_liberations = atoi(argv[2]);
	
	if (n_reservations < n_liberations) {
		n_liberations = n_reservations;
		fprintf(stderr, "%sAVISO:%s No puede haber más hilos de reservas que de liberaciones.\n", COLOR_RED, COLOR_RESET);
	}
	
	pthread_t * reservation_threads;
	if ((reservation_threads=malloc(n_reservations*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos de reservas: %s\n", strerror(errno));
		exit(-1);
	}
	
	pthread_t * liberation_threads;
	if ((liberation_threads=malloc(n_liberations*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos de liberaciones: %s\n", strerror(errno));
		exit(-1);
	}
	
	if (crea_sala(CAPACIDAD_SALA) == -1) {
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}
	
	int reservation_args[n_reservations];
	for (int i = 0; i < n_reservations; i++) {
		reservation_args[i] = i+1;
		if (ret = pthread_create(&reservation_threads[i], NULL, hilo_reservas, &reservation_args[i])) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo de reservas %d: %s\n", i, strerror(errno));
		}
	}
	
	int liberation_args[n_liberations];
	for (int i = 0; i < n_liberations; i++) {
		liberation_args[n_liberations] = i+1;	
		if (ret = pthread_create(&liberation_threads[i], NULL, hilo_liberacion, &liberation_args[i])) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo de liberaciones %d: %s\n", i, strerror(errno));
		}
	}
	
	pthread_t state_thread;
	if (ret = pthread_create(&state_thread, NULL, hilo_estado, NULL)) {
		errno = ret;
		fprintf(stderr, "Error al crear el hilo estado: %s\n", strerror(errno));
		exit(-1);
	}
	
	for (int i = 0; i < n_reservations; i++) {
		if (ret = pthread_join(reservation_threads[i], NULL)) {
			errno = ret;
			fprintf(stderr, "Error al esperar por el hilo de reservas %d: %s\n", i, strerror(errno));
			exit(-1);
		}
	}
	
	for (int i = 0; i < n_liberations; i++) {
		if (ret = pthread_join(liberation_threads[i], NULL)) {
			errno = ret;
			fprintf(stderr, "Error al esperar por hilo de liberaciones %d: %s\n", i, strerror(errno));
			exit(-1);
		}
	}
	

	pthread_cancel(state_thread);

	estado_sala();
	elimina_sala();
	
	free(reservation_threads);
	free(liberation_threads);
	
	exit(0);
}

