#include <stdio.h>
#include <string.h>
#include <signal.h>	
#include "sucursal.h"

int main(int argc, char argv[]) {
	sigset_t set;
	signal (SIGCHLD, manejador_sala_terminada);
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);
	
	while (1) {
		sigprocmask(SIG_BLOCK, &set, NULL);
		char ciudad[20] = "";
		char capacidad[3] = ""; // Max capacidad = 999
		
		while(!strcmp(ciudad, "")) {
			printf("Ciudad: ");
			fgets (ciudad, sizeof(ciudad), stdin);
			ciudad[strcspn(ciudad, "\n")] = 0;
		}
		if (!strcmp(ciudad, "salir")) break;
		while(!strcmp(capacidad, "")) {
			printf("Capacidad: ");
			fgets (capacidad, sizeof(capacidad), stdin);
			capacidad[strcspn(capacidad, "\n")] = 0;
		}
		
		crea_sucursal(ciudad, capacidad);
		printf("\n");
		sigprocmask(SIG_UNBLOCK, &set, NULL);
	}
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

