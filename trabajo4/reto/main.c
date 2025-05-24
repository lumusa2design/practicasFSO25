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


pthread_mutex_t seatMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t freeCond = PTHREAD_COND_INITIALIZER;
int free_seats = 0;
int premium_seats = 0;

int n_premium;
int n_free;


void* hilo_reservas_pago(void* arg) {
	int id_hilo = *(int*)arg;
	int base = id_hilo*10;
	
	printf("%sHilo %d:%s Reservando asiento para id %d...\n", COLOR_GREEN, id_hilo, COLOR_RESET, base);
	int resultado = reserva_asiento(base);
	if (resultado != -1) {
		printf("%sHilo %d:%s Asiento %d reservado para id %d.\n", COLOR_GREEN, id_hilo, COLOR_RESET, resultado, base);
	} else {
		printf("%sHilo %d:%s ERROR: No se ha podido reservar el asiento para la id %d.\n%s", COLOR_GREEN, id_hilo, COLOR_RED, base, COLOR_RESET);
	}
	pthread_mutex_lock(&seatMutex);
	premium_seats++;
	pthread_cond_signal(&freeCond);
	pthread_mutex_unlock(&seatMutex);
	printf("%sHilo %d finalizado.\n%s", COLOR_GREEN, id_hilo, COLOR_RESET);
}

void* hilo_liberaciones_pago(void*arg) {
	int id_hilo = *(int*)arg;
	int base = id_hilo*10;
	
	printf("%sHilo %d:%s Liberando asiento...\n", COLOR_GREEN, id_hilo, COLOR_RESET);
	int asiento = libera_asiento();
	if (asiento == -1) {
		printf("%sHilo %d:%s ERROR: No se ha podido liberar un asiento.\n%s", COLOR_GREEN, id_hilo, COLOR_RED, COLOR_RESET);
	} else {
		printf("%sHilo %d:%s Asiento %d liberado.\n", COLOR_GREEN, id_hilo, COLOR_RESET, asiento);
	}
	pthread_mutex_lock(&seatMutex);
	premium_seats--;
	pthread_cond_signal(&freeCond);
	pthread_mutex_unlock(&seatMutex);
	printf("%sHilo %d finalizado.\n%s", COLOR_GREEN, id_hilo, COLOR_RESET);
}


void* hilo_reservas_gratis(void* arg) {
	int id_hilo = *(int*)arg;
	int base = id_hilo*10;
	
	pthread_mutex_lock(&seatMutex);
	while(premium_seats == 0) {
		pthread_cond_wait(&freeCond, &seatMutex);
	}
	while(n_premium && (float)free_seats/premium_seats*100 > 10) {
		pthread_cond_wait(&freeCond, &seatMutex);
	}
	
	printf("%sHilo %d:%s Reservando asiento para id %d...\n", COLOR_YELLOW, id_hilo, COLOR_RESET, base);
	int resultado = reserva_asiento(base);
	if (resultado != -1) {
		printf("%sHilo %d:%s Asiento %d reservado para id %d.\n", COLOR_YELLOW, id_hilo, COLOR_RESET, resultado, base);
	} else {
		printf("%sHilo %d:%s ERROR: No se ha podido reservar el asiento para la id %d.\n%s", COLOR_YELLOW, id_hilo, COLOR_RED, base, COLOR_RESET);
	}
	free_seats++;
	pthread_mutex_unlock(&seatMutex);
	printf("%sHilo %d finalizado.\n%s", COLOR_YELLOW, id_hilo, COLOR_RESET);
}

void* hilo_liberaciones_gratis(void*arg) {
	int id_hilo = *(int*)arg;
	int base = id_hilo*10;
	
	printf("%sHilo %d:%s Liberando asiento...\n", COLOR_YELLOW, id_hilo, COLOR_RESET);
	int asiento = libera_asiento();
	if (asiento == -1) {
		printf("%sHilo %d:%s ERROR: No se ha podido liberar un asiento.\n%s", COLOR_YELLOW, id_hilo, COLOR_RED, COLOR_RESET);
	} else {
		printf("%sHilo %d:%s Asiento %d liberado.\n", COLOR_YELLOW, id_hilo, COLOR_RESET, asiento);
	}
	pthread_mutex_lock(&seatMutex);
	free_seats--;
	pthread_cond_signal(&freeCond);
	pthread_mutex_unlock(&seatMutex);
	printf("%sHilo %d finalizado.\n%s", COLOR_YELLOW, id_hilo, COLOR_RESET);
}


void* hilo_estado(void* args) {
	while(1) {
		estado_sala();
		printf("Asientos de pago: %d\n Asientos gratis %d\n", premium_seats, free_seats);
		printf("n_premium: %d\n", n_premium);
	}
}


int ret;
int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Uso: ./ n_hilos_gratis n_hilos_pago\n");
		exit(-1);
	}
	
	
	n_free = atoi(argv[1]);
	if (n_free < 1) {
		fprintf(stderr, "Error: El número de hilos debe ser mayor que 0. Número introducido: %d", n_free);
	}
	n_premium = atoi(argv[2]);
	if (n_free < 1) {
		fprintf(stderr, "Error: El número de hilos debe ser mayor que 0. Número introducido: %d", n_free);
	}
	

	int n_threads = n_free + n_premium;
	pthread_t * threads;
	if ((threads=malloc(n_threads*sizeof(pthread_t))) == NULL) {
		fprintf(stderr, "Error al asignar memoria a los hilos de reservas: %s\n", strerror(errno));
		exit(-1);
	}
	
	
	if (crea_sala(CAPACIDAD_SALA) == -1) {
		fprintf(stderr, "Error al crear la sala.\n");
		exit(-1);
	}
	
	
	
	
	int args[n_threads];
	int reservations = 0;
	int liberations = 0;
	for (int i = 0; i < n_premium; i++) {
		args[i] = i+1;
		int thread_op = rand()%2;
		
		if (thread_op || liberations >= reservations) {
			if (ret = pthread_create(&threads[i], NULL, hilo_reservas_pago, &args[i])) {
				errno = ret;
				fprintf(stderr, "Error al crear hilo: %s\n", strerror(errno));
				exit(-1);
			}
			reservations++;
		} else {
			if (ret = pthread_create(&threads[i], NULL, hilo_liberaciones_pago, &args[i])) {
				errno = ret;
				fprintf(stderr, "Error al crear hilo: %s\n", strerror(errno));
				exit(-1);
			}
			liberations++;
		}
	}
	for (int i = n_premium; i<n_threads; i++) {
		args[i] = i+1;
		int thread_op = rand()%2;
		
		if (thread_op || liberations >= reservations) {
			if (ret = pthread_create(&threads[i], NULL, hilo_reservas_gratis, &args[i])) {
				errno = ret;
				fprintf(stderr, "Error al crear hilo: %s\n", strerror(errno));
				exit(-1);
			}
			reservations++;
		} else {
			if (ret = pthread_create(&threads[i], NULL, hilo_liberaciones_gratis, &args[i])) {
				errno = ret;
				fprintf(stderr, "Error al crear hilo: %s\n", strerror(errno));
				exit(-1);
			}
			liberations++;
		}
	}
	
	pthread_t state_thread;
	if (ret = pthread_create(&state_thread, NULL, hilo_estado, NULL)) {
		errno = ret;
		fprintf(stderr, "Error al crear el hilo estado: %s\n", strerror(errno));
		exit(-1);
	}
	
	
	for (int i = 0; i < n_threads; i++) {
		if (ret = pthread_join(threads[i], NULL)) {
			errno = ret;
			fprintf(stderr, "Error al esperar por el hilo de reservas %d: %s\n", i, strerror(errno));
			exit(-1);
		}
		if (n_premium > 0) {
			n_premium--;
			pthread_cond_signal(&freeCond);
		}
	}
	

	pthread_cancel(state_thread);

	estado_sala();
	elimina_sala();
	
	free(threads);
	pthread_mutex_destroy(&seatMutex);
	pthread_cond_destroy(&freeCond);
	
	exit(0);
}

