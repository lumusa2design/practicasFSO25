#ifndef SALA_H_
#define SALA_H_

int reserva_asiento(int id_persona);
int libera_asiento(int id_asiento);
int estado_asiento(int id_asiento);
int asientos_libres();
int asientos_ocupados();
int capacidad_sala();
int crea_sala(char nombre[20], int capacidad);
int elimina_sala();
char * nombre_sala();
void estado_sala();
int guarda_estado_sala(const char* ruta_fichero, int existe);
int recupera_estado_sala(const char* ruta_fichero);
int guarda_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos);
int recupera_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos);
int anula(int id_persona);
int compara(const char* ruta_fichero1, const char* rutafichero2);

#endif /* SALA_H_ */
