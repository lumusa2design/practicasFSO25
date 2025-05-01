#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sala.h"

int main (int argc, char *argv[]) {
	extern char *optarg;
	extern int optind, opterr, optopt;
	
	int param = 0, err = 0;
	int fflag = 0, cflag = 0, oflag = 0;
	char *route;
	int capacity;
	
	if (argc == 1) {
		fprintf(stderr, "Uso: %s comando [argumentos].\nEscribe \"%s ayuda\" para ver la lista de comandos.\n", argv[0], argv[0]);
		exit(-1);
	}
	
	if (!strcmp("crea", argv[1])) {
		while ((param = getopt(argc, argv, "f:oc:")) != -1) {
			switch(param) {
				case 'f':
					if (fflag == 1) {
						err = 1;
						break;
					}
					fflag = 1;
					route = optarg;
					break;
				case 'c':
					if (cflag == 1){
						err = 1;
						break;
					}
					cflag = 1;
					capacity = atoi(optarg);
					break;
				case 'o':
					if (oflag == 1) {
						err = 1;
						break;
					}
					oflag = 1;
					break;
				default:
					err = 1;
					break;
			}
		}
		if (fflag == 0 || cflag == 0 || err == 1) {
			fprintf(stderr, "Uso: %s crea -f ruta [-o] -c capacidad\n", argv[0]);
			exit(-1);
		}
		
		if (crea_sala("", capacity) == -1) {
			fprintf(stderr, "Error al crear la sala.\n");
			exit(-1);
		}	
		
		if (guarda_estado_sala(route, oflag) == -1) {
			fprintf(stderr, "Error al guardar el estado de la sala.\n");
			exit(-1);
		}
		
		printf("Sala creada con capacidad %d en %s\n", capacity, route);
		
		
	} else if (!strcmp("reserva", argv[1])) {
		int *ids;
		int n_ids;
		int *seat_ids;
		while ((param = getopt(argc, argv, "f:")) != -1) {
			switch (param) {
				case 'f':
					if (fflag == 1) {
						err = 1;
					}
					fflag = 1;
					
					route = optarg;
					
					n_ids = argc-optind;
					if (n_ids <= 0) {
						err = 1;
						break;
					}
					ids = malloc(n_ids*sizeof(int));
					if (ids == NULL) {
						fprintf(stderr, "Error al asignar memoria para la lista de ids.\n");
						exit(-1);
					}
					seat_ids = malloc(n_ids*sizeof(int));
					if (seat_ids == NULL) {
						fprintf(stderr, "Error al asignar memoria para la lista de asientos reservados.\n");
						exit(-1);
					}
					for (int i = 0; i < n_ids; i++) {
						ids[i] = atoi(argv[i+optind]);
					}
					
					break;
				default:
					err = 1;
					break;
			}
		}
		if (fflag == 0 || err == 1) {
			fprintf(stderr, "Uso: %s reserva -f ruta id_persona1 id_persona2 ... id_personaN\n", argv[0]);
			exit(-1);
		}
		
		if (recupera_estado_sala(route) == -1) {
			fprintf(stderr, "Error al recuperar la sala.\n");
			exit(-1);
		}
		
		for (int i = 0; i < n_ids; i++) {
			if (seat_ids[i] = reserva_asiento(ids[i]) == -1) {
				fprintf(stderr, "Error al reservar asiento.\n");
				exit(-1);
			}
		}
		if (guarda_estado_sala(route, 1) == -1) {
			fprintf(stderr, "Error al guardar los datos.\n");
			exit(-1);
		}
		free(ids);
		free(seat_ids);
		printf("Reserva hecha correctamente.\n");
		
		
	} else if (!strcmp("anula", argv[1])) {
		// TODO
		
	} else if (!strcmp("estado", argv[1])) {
		while ((param = getopt(argc, argv, "f:")) != -1) {
			switch (param) {
				case 'f':
					if (fflag == 1) {
						err = 1;
						break;
					}
					fflag = 1;
					route = optarg;
					break;
				case '?':
					err = 1;
					break;
			}
		}
		if (fflag == 0 || err == 1) {
			fprintf(stderr, "Uso: %s estado -f ruta\n", argv[0]);
			exit(-1);
		}
		
		if (recupera_estado_sala(route) == -1) {
			fprintf(stderr, "Error al recuperar la sala.\n");
			exit(-1);
		}
		estado_sala();
		
		
	} else if (!strcmp("ayuda", argv[1])) {
		if (argc != 2) {
			fprintf(stderr, "Uso: %s ayuda\n", argv[0]);
			exit(-1);
		}
		printf("Lista de comandos:\n");
		printf("%s crea -f ruta [-o] -c capacidad\n", argv[0]);
		printf("%s reserva -f ruta id_persona1 [id_persona2 ... id_personaN]\n", argv[0]);
		printf("%s anula -f ruta id_asiento1 [id_asiento2 ... id_asientoN]\n", argv[0]);
		printf("%s estado -f ruta\n", argv[0]);
		printf("%s ayuda\n", argv[0]);

		
	} else {
		fprintf(stderr, "Comando desconocido. Escribie \"%s ayuda\" para ver los comandos disponibles.\n", argv[0]);
		exit(-1);
	}
	
	exit(0);
}
