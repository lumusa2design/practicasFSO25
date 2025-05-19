#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "sala.h"
#include "retardo.h"

#define ITERACIONES 3
#define CAPACIDAD 20
#define MAX_PAUSA 1.5

void* hilo_reservas(void* arg) {
    int base = (int)(size_t)arg;
    for (int i = 0; i < ITERACIONES; i++) {
        pausa_aleatoria(MAX_PAUSA);
        int asiento = reserva_asiento(base + i);
        printf("Reserva: id %d -> asiento %d\n", base + i, asiento);
    }
    return NULL;
}

void* hilo_liberaciones(void* arg) {
    (void)arg;
    for (int i = 0; i < ITERACIONES; i++) {
        pausa_aleatoria(MAX_PAUSA);
        int total = capacidad_sala();
        int id_asiento = rand() % total;
        int res = libera_asiento(id_asiento);
        printf("Liberación: asiento %d -> resultado %d\n", id_asiento, res);
    }
    return NULL;
}

void* hilo_estado(void* arg) {
    (void)arg;
    while (1) 
    {
        estado_sala();
        sleep(2);
    }
    return NULL;
}

int main(int argc, char* argv[])
 {
    	if (argc != 3) 
 	{
		fprintf(stderr, "Uso: %s n m\n", argv[0]);
		exit(EXIT_FAILURE);
 	}

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    if (n < 1 || m < 1) {
        fprintf(stderr, "n y m deben ser mayores que 0\n");
        exit(EXIT_FAILURE);
    }

    crea_sala(CAPACIDAD);

    pthread_t hilos_reserva[n], hilos_libera[m], hilo_estado_id;

    for (int i = 0; i < n; i++)
        pthread_create(&hilos_reserva[i], NULL, hilo_reservas, (void*)(size_t)(i * 10));

    for (int i = 0; i < m; i++)
    	{
        	pthread_create(&hilos_libera[i], NULL, hilo_liberaciones, NULL);
	}
	
    pthread_create(&hilo_estado_id, NULL, hilo_estado, NULL);

    for (int i = 0; i < n; i++) {
    	pthread_join(hilos_reserva[i], NULL);
    	}
    	
    for (int i = 0; i < m; i++) 
	{
	    pthread_join(hilos_libera[i], NULL);
	}
	
    pthread_cancel(hilo_estado_id);
    elimina_sala();
    return 0;
}

