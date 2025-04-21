#ifndef SALA_H_
#define SALA_H_

# define MAX_CIUDAD_LEN 20

int reserva_asiento(int id_persona);
int libera_asiento(int id_asiento);
int estado_asiento(int id_asiento);
int asientos_libres();
int asientos_ocupados();
int capacidad_sala();
int crea_sala(char nombre[MAX_CIUDAD_LEN], int capacidad);
int elimina_sala();
char * nombre_sala();
int guarda_estado_sala(const char* ruta_fichero);
int recupera_estado_sala(const char* ruta_fichero);
int guarda_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos);
int recupera_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos);


#endif /* SALA_H_ */
