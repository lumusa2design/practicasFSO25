#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "sala.h"

int main(int argc, char *argv[]) {
	extern char *optarg;
	extern int optind, opterr, optopt;

	int param = 0;
	int err = 0;
	int fflag = 0, cflag = 0, oflag = 0;
	char *route = NULL;
	int capacity;

	if (argc == 1) {
		fprintf(stderr, "Uso: %s comando [argumentos].\nEscribe \"%s ayuda\" para ver la lista de comandos.\n", argv[0], argv[0]);
		exit(-1);
	}

	if (!strcmp("crea", argv[1])) {
		while ((param = getopt(argc, argv, "f:oc:")) != -1) {
			switch (param) {
				case 'f': fflag = 1; route = optarg; break;
				case 'c': cflag = 1; capacity = atoi(optarg); break;
				case 'o': oflag = 1; break;
				default: err = 1; break;
			}
		}
		if (!fflag || !cflag || err) {
			fprintf(stderr, "Uso: %s crea -f ruta [-o] -c capacidad\n", argv[0]); exit(-1);
		}
		if (crea_sala("", capacity) == -1 || guarda_estado_sala(route, oflag) == -1) {
			fprintf(stderr, "Error al crear o guardar la sala.\n"); exit(-1);
		}
		printf("Sala creada con capacidad %d en %s\n", capacity, route);

	} else if (!strcmp("reserva", argv[1])) {
		int *ids = NULL, *seat_ids = NULL, n_ids = 0;
		while ((param = getopt(argc, argv, "f:")) != -1) {
			if (param == 'f') {
				fflag = 1; route = optarg;
				n_ids = argc - optind;
				if (n_ids <= 0) { err = 1; break; }
				ids = malloc(n_ids * sizeof(int));
				seat_ids = malloc(n_ids * sizeof(int));
				if (!ids || !seat_ids) { fprintf(stderr, "Error de memoria.\n"); exit(-1); }
				for (int i = 0; i < n_ids; i++) ids[i] = atoi(argv[i + optind]);
			} else err = 1;
		}
		if (!fflag || err) {
			fprintf(stderr, "Uso: %s reserva -f ruta id_persona1 [id_persona2 ...]\n", argv[0]); exit(-1);
		}
		if (recupera_estado_sala(route) == -1) { fprintf(stderr, "Error al recuperar la sala.\n"); exit(-1); }
		for (int i = 0; i < n_ids; i++) {
			seat_ids[i] = reserva_asiento(ids[i]);
			if (seat_ids[i] == -1) {
				fprintf(stderr, "Error al reservar asiento para ID %d.\n", ids[i]); exit(-1);
			}
		}
		if (guarda_estado_sala(route, 1) == -1) { fprintf(stderr, "Error al guardar los datos.\n"); exit(-1); }
		free(ids); free(seat_ids); printf("Reserva hecha correctamente.\n");

	} else if (!strcmp("anula", argv[1]) && argc > 4 && strcmp(argv[3], "-personas") == 0) {
		char *ruta = argv[2];
		int n_ids = argc - 4;
		int *ids = malloc(n_ids * sizeof(int));
		if (!ids) { fprintf(stderr, "Error de memoria.\n"); exit(-1); }
		for (int i = 0; i < n_ids; i++) ids[i] = atoi(argv[i + 4]);
		if (recupera_estado_sala(ruta) == -1) { fprintf(stderr, "Error al recuperar la sala.\n"); exit(-1); }
		for (int i = 0; i < n_ids; i++) {
			if (anula(ids[i]) == -1) {
				fprintf(stderr, "No se encontró reserva para el ID de persona %d.\n", ids[i]);
			}
		}
		if (guarda_estado_sala(ruta, 1) == -1) { fprintf(stderr, "Error al guardar los datos.\n"); exit(-1); }
		free(ids); printf("Anulación completada.\n");

	} else if (!strcmp("compara", argv[1])) {
		if (argc != 4) {
			fprintf(stderr, "Uso: %s compara ruta1 ruta2\n", argv[0]); exit(-1);
		}
		int resultado = compara(argv[2], argv[3]);
		if (resultado == -1) {
			fprintf(stderr, "Error comparando los archivos.\n"); exit(-1);
		}
		exit(resultado);

	} else if (!strcmp("estado", argv[1])) {
		while ((param = getopt(argc, argv, "f:")) != -1) {
			if (param == 'f') {
				if (fflag++) { err = 1; break; }
				route = optarg;
			} else err = 1;
		}
		if (!fflag || err) {
			fprintf(stderr, "Uso: %s estado -f ruta\n", argv[0]); exit(-1);
		}
		if (recupera_estado_sala(route) == -1) {
			fprintf(stderr, "Error al recuperar la sala.\n"); exit(-1);
		}
		estado_sala();

	} else if (!strcmp("ayuda", argv[1])) {
		printf("Lista de comandos:\n");
		printf("%s crea -f ruta [-o] -c capacidad\n", argv[0]);
		printf("%s reserva -f ruta id_persona1 [id_persona2 ...]\n", argv[0]);
		printf("%s anula -f ruta id_asiento1 [id_asiento2 ...]\n", argv[0]);
		printf("%s anula ruta -personas id1 [id2 ...]\n", argv[0]);
		printf("%s compara ruta1 ruta2\n", argv[0]);
		printf("%s estado -f ruta\n", argv[0]);
		printf("%s ayuda\n", argv[0]);
	} else {
		fprintf(stderr, "Comando desconocido. Escribe \"%s ayuda\" para ver los comandos disponibles.\n", argv[0]); exit(-1);
	}

	exit(0);
}

