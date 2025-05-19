#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "sala.h"
#include "retardo.h"

#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_RESET "\033[0m"

#define ITERACIONES_HILO 3
#define MAX_PAUSA 0.1
#define CAPACIDAD_SALA 20

void* hilo_reservas(void* arg) {
	int asientos[ITERACIONES_HILO];
	int id_hilo = (int)arg;
	int base = id_hilo*10;
	
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		pausa_aleatoria(MAX_PAUSA);
		printf("%sHilo %d:%s Reservando asiento para id %d...\n", COLOR_YELLOW, id_hilo, COLOR_RESET, i+base);
		asientos[i] = reserva_asiento(i+base);
		if (asientos[i] != -1) {
			printf("%sHilo %d:%s Asiento %d reservado para id %d.\n", COLOR_YELLOW, id_hilo, COLOR_RESET, asientos[i], i+base);
		estado_sala();
		} else {
			printf("%sHilo %d:%s ERROR: No se ha podido reservar el asiento para la id %d.\n%s", COLOR_YELLOW, id_hilo, COLOR_RED, i+base, COLOR_RESET);
		}
	}
	
	for(int i = 0; i < ITERACIONES_HILO; i++) {
		pausa_aleatoria(MAX_PAUSA);
		if (asientos[i] != -1) {
			printf("%sHilo %d:%s Liberando asiento %d...\n", COLOR_YELLOW, id_hilo, COLOR_RESET, asientos[i]);
			if (libera_asiento(asientos[i]) == -1) {
				printf("%sHilo %d:%s ERROR: No se ha podido liberar el asiento con id %d.\n%s", COLOR_YELLOW, id_hilo, COLOR_RED, asientos[i], COLOR_RESET);
			} else {
				printf("%sHilo %d:%s Asiento %d liberado.\n", COLOR_YELLOW, id_hilo, COLOR_RESET, asientos[i]);
		estado_sala();
			}
		}
	}
}

void* hilo_estado(void* args) {
	while(1) {
		estado_sala();
		pausa_aleatoria(MAX_PAUSA);
	}
}


int ret;
int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Uso: ./ numero_de_hilos\n");
		exit(-1);
	}
	
	
	int n_threads = atoi(argv[1]);
	
	pthread_t * threads;
	if ((threads=malloc(n_threads*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos: %s\n", strerror(errno));
		exit(-1);
	}
	
	if (crea_sala(CAPACIDAD_SALA) == -1) {
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}
	
	for (int i = 0; i < n_threads; i++) {
		int arg = i+1;	
		if (ret = pthread_create(&threads[i], NULL, hilo_reservas, (void*)arg)) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo %d: %s\n", i, strerror(errno));
		}
	}
	
	/*pthread_t state_thread;
	if (ret = pthread_create(&state_thread, NULL, hilo_estado, NULL)) {
		errno = ret;
		fprintf(stderr, "Error al crear el hilo estado: %s\n", strerror(errno));
		exit(-1);
	}*/
	
	for (int i = 0; i < n_threads; i++) {
		if (ret = pthread_join(threads[i], NULL)) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo %d: %s\n", i, strerror(errno));
			exit(-1);
		}
	}
	
	// pthread_cancel(state_thread);
	
	estado_sala();
	elimina_sala();
	
	free(threads);
	
	exit(0);
}

