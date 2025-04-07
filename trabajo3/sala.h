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

#endif /* SALA_H_ */
