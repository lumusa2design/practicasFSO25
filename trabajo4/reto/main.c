#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "sala.h"

#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_RESET "\033[0m"

#define ITERACIONES_HILO 3
#define CAPACIDAD_SALA 20


struct thread_arg {
		int id;
		int isPremium;
};


// TODO: Lógica premium vs gratis
// TODO: Implememntar libera asiento(int id_asiento) como estaba antes

void* hilo_reservas(void* arg) {
	
	struct thread_arg* args= (struct thread_arg*) arg;
	
	int id_hilo = args->id;
	int base = id_hilo*10;
	int pago = args->isPremium;
	
	char* color = pago?COLOR_GREEN:COLOR_YELLOW;
	
	
	printf("%sHilo reservas %d:%s Reservando asiento para id %d...\n", color, id_hilo, COLOR_RESET, i+base);
	int resultado = reserva_asiento(i+base);
	if (resultado != -1) {
		printf("%sHilo reservas %d:%s Asiento %d reservado para id %d.\n", color, id_hilo, COLOR_RESET, resultado, i+base);
	} else {
		printf("%sHilo reservas %d:%s ERROR: No se ha podido reservar el asiento para la id %d.\n%s", color, id_hilo, COLOR_RED, i+base, COLOR_RESET);
	}
	} else {
	
	}
	printf("%sHilo reservas %d finalizado.\n%s", color, id_hilo, COLOR_RESET);
}

void* hilo_liberaciones(void*arg) {
	struct thread_arg* args= (struct thread_arg*) arg;
	
	int id_hilo = args->id;
	int base = id_hilo*10;
	int pago = args->isPremium;
	
	char* color = pago?COLOR_GREEN:COLOR_YELLOW;
	
	printf("%sHilo liberaciones %d:%s Liberando asiento...\n", color, id_hilo, COLOR_RESET);
	int asiento = libera_asiento();
	if (asiento == -1) {
		printf("%sHilo liberaciones %d:%s ERROR: No se ha podido liberar un asiento.\n%s", color, id_hilo, COLOR_RED, COLOR_RESET);
	} else {
		printf("%sHilo liberaciones %d:%s Asiento %d liberado.\n", color, id_hilo, COLOR_RESET, asiento);
	}	
	printf("%sHilo liberaciones %d finalizado.\n%s", color, id_hilo, COLOR_RESET);
}

void* hilo_estado(void* args) {
	while(1) {
		estado_sala();
	}
}


int ret;
int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Uso: ./ n_hilos_gratis n_hilos_pago\n");
		exit(-1);
	}
	
	
	int n_free = atoi(argv[1]);
	if (n_free < 1) {
		fprintf(stderr, "Error: El número de hilos debe ser mayor que 0. Número introducido: %d", n_free);
	}
	int n_premium = atoi(argv[2]);
	if (n_free < 1) {
		fprintf(stderr, "Error: El número de hilos debe ser mayor que 0. Número introducido: %d", n_free);
	}
	

	int n_threads = n_free + n_premium;
	pthread_t * reservation_threads;
	if ((reservation_threads=malloc(n_threads*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos de reservas: %s\n", strerror(errno));
		exit(-1);
	}
	
	pthread_t * liberation_threads;
	if ((liberation_threads=malloc(n_threads*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos de liberaciones: %s\n", strerror(errno));
		exit(-1);
	}
	
	if (crea_sala(CAPACIDAD_SALA) == -1) {
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}
	
	struct thread_arg args[n_threads];
	for (int i = 0; i < n_threads; i++) {
		
		args[i].id = i+1;
		args[i].isPremium = i<n_premium?1:0;
		
		if (ret = pthread_create(&reservation_threads[i], NULL, hilo_reservas, &args[i])) {
			errno = ret;
			fprintf(stderr, "Error al crear el hilo de reservas %d: %s\n", i, strerror(errno));
		}
		
		if (ret = pthread_create(&liberation_threads[i], NULL, hilo_liberaciones, &args[i])) {
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
	
	for (int i = 0; i < n_threads; i++) {
		if (ret = pthread_join(reservation_threads[i], NULL)) {
			errno = ret;
			fprintf(stderr, "Error al esperar por el hilo de reservas %d: %s\n", i, strerror(errno));
			exit(-1);
		}
		
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

