#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sala.h"

int main (int argc, char * argv[]) {
	if (!strcmp("crea", argv[1])) {
	
	} else if (!strcmp("reserva", argv[1])) {
	
	} else if (!strcmp("anula", argv[1])) {
	
	} else if (!strcmp("estado", argv[1])) {
	
	} else if (!strcmp("ayuda", argv[1])) {
		if (argc != 2) {
			fprintf(stderr, "Uso: %s ayuda\n", argv[0]);
			exit(-1);
		}
		
		
	} else {
		fprintf(stderr, "Comando desconocido. Escribie \"%s ayuda\" para ver los comandos disponibles.\n", argv[0]);
		exit(-1);
	}
}
