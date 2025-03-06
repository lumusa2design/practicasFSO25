// test_sala.c
// ===============
// Batería de pruebas de la biblioteca "sala.h/sala.c"
//

#include <assert.h>
#include <stdio.h>
#include "sala.h"

#define DebeSerCierto(x)	assert(x)
#define DebeSerFalso(x)		assert(!(x))

#define CAPACIDAD 20

void INICIO_TEST (const char* titulo_test) {
  printf("********** batería de pruebas para %s:\n", titulo_test); 
 	// fflush fuerza que se imprima el mensaje anterior
	// sin necesidad de utilizar un salto de línea
	fflush(stdout);
}

void FIN_TEST (const char* titulo_test) {
  printf("********** hecho\n");
}


/*void test_ReservaBasica() {
	int mi_asiento;
	#define CAPACIDAD 500
	#define ID_1 1500

	INICIO_TEST("Reserva básica");
	crea_sala(CAPACIDAD);
	DebeSerCierto(capacidad()==CAPACIDAD);
	DebeSerCierto((mi_asiento=reserva_asiento(ID_1))>=0);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD);
	DebeSerCierto(estado_asiento(mi_asiento)>0);
	DebeSerCierto(libera_asiento(mi_asiento)==ID_1);
	DebeSerCierto((asientos_libres()+asientos_ocupados())==CAPACIDAD);
	elimina_sala();
	FIN_TEST("Reserva básica");
}*/

void test_CreaSala() {
	INICIO_TEST("Crea Sala");
	DebeSerCierto(crea_sala(CAPACIDAD) == CAPACIDAD);
	elimina_sala();
	FIN_TEST("Crea Sala");
}

void test_CreaSalaSinEliminar() {
	INICIO_TEST("Crea Sala Sin Eliminar");
	crea_sala(CAPACIDAD);
	DebeSerCierto(crea_sala(CAPACIDAD) == -1);
	elimina_sala();
	FIN_TEST("Crea Sala Sin Eliminar");
}

void test_CreaSalaCapacidadNegativa() {
	INICIO_TEST("Crea Sala Capacidad Negativa");
	DebeSerCierto(crea_sala(CAPACIDAD*-1) == -1);
	elimina_sala();
	FIN_TEST("Crea Sala");
}

void test_CreaSalaCapacidadCero() {
	INICIO_TEST("Crea Sala Capacidad Cero");
	DebeSerCierto(crea_sala(0) == -1);
	elimina_sala();
	FIN_TEST("Crea Sala");
}

void test_EliminaSalaInexistente() {
	INICIO_TEST("Elimina Sala Inexistente");
	DebeSerCierto(elimina_sala() == -1);
	FIN_TEST("Elimina Sala Sin Sala");
}

void test_EliminaSala() {
	INICIO_TEST("Elimina Sala");
	crea_sala(CAPACIDAD);
	DebeSerCierto(elimina_sala() == 0);
	FIN_TEST("Elimina Sala");
}

void ejecuta_tests() {
	printf("Iniciando tests...\n");
	
	// test_ReservaBasica();
	// Añadir nuevos tests 
	test_CreaSala();
	test_CreaSalaSinEliminar();
	test_CreaSalaCapacidadNegativa();
	test_CreaSalaCapacidadCero();
	test_EliminaSalaInexistente();
	test_EliminaSala();
	printf("Betería de test completa.\n");
}

/* main() {
 *	puts("Iniciando tests...");
 *	
 *	ejecuta_tests();
 *	
 *	puts("Batería de test completa.");
 *}
 */

