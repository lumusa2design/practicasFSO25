#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>


# define ASIENTO_LIBRE 0
# define MAX_CIUDAD_LEN 20


struct sala {
	char ciudad[MAX_CIUDAD_LEN];
	int capacidad;
	int libres;
	int asientos[];
};


int existe_sala();
int capacidad_sala();
int estado_asiento(int id_asiento);
int digitos_asiento();
int posicion_cursor_asiento(int id_asiento);

struct sala *miSala = NULL;

int libera_asiento(int id_asiento)
{
	if(!existe_sala())
	{
		fprintf(stderr, "La sala no existe\n");
		return -1;
	}
	
	if(id_asiento >= capacidad_sala() || id_asiento < 0 ) 
	{
		fprintf(stderr,"El asiento %d no existe\n", id_asiento);
		return -1;
	}
	if(estado_asiento(id_asiento) != ASIENTO_LIBRE) 
	{
		int estado_old = estado_asiento(id_asiento);
		miSala->libres ++;
		miSala->asientos[id_asiento] = ASIENTO_LIBRE;
		return estado_old;
	} else 
	{
		fprintf(stderr, "El asiento ya estaba vacío.\n");
		return -1;
	}	
}


int reserva_asiento(int id_persona)
{
	if(!existe_sala()) 
	{
		fprintf(stderr,"la sala no existe\n");
		return -1;
	} 
	if(id_persona < 0) 
	{
		fprintf(stderr, "El id de la persona tiene que ser positivo\n");
		return -1;
	}
	if(id_persona == ASIENTO_LIBRE)
	{
		fprintf(stderr, "El id introducido no es válido. Id introducido: %imisala\n", id_persona);
		return -1;
	}
	for(int i = 0; i <capacidad_sala(); i++)
	{
		if(estado_asiento(i) != -1 && estado_asiento(i) == ASIENTO_LIBRE) 
		{
			miSala->asientos[i] = id_persona;
			miSala->libres --;
			return i;
		}
	}
	fprintf(stderr, "La sala está llena.\n");
	return -1;
}


int estado_asiento(int id_asiento) {
	if (!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	if (id_asiento > capacidad_sala() - 1 || id_asiento < 0) {
		fprintf(stderr,"El asiento %i no existe.\n", id_asiento);
		return -1;
	}
	return miSala->asientos[id_asiento];
}


int asientos_libres() {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe.\n");
		return -1;
	}
	

	return miSala->libres;
}


int capacidad_sala() {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe.\n");
		return -1;
	}
	return miSala->capacidad;
}


int asientos_ocupados () {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe.\n");
		return -1;
	}
	
	return capacidad_sala() - asientos_libres();
}


int crea_sala(char nombre[MAX_CIUDAD_LEN], int capacidad) {
	if (existe_sala()) {
		fprintf(stderr, "La sala ya está creada.\n");
		return -1;
	}
	
	if (capacidad <= 0) {
		fprintf(stderr, "La capacidad debe ser mayor que 0. Capacidad introducida: %i\n", capacidad);
		return -1;
	}
	
	miSala = (struct sala *) malloc(sizeof(struct sala) + capacidad * sizeof(int));
	if(miSala == NULL) 
	{
		fprintf(stderr,"Error al crear sala\n");
		return -1;
	}
	if (!existe_sala()) {
		fprintf(stderr, "Error de memoria.\n");
		return -1;
	}
	
	strcpy(miSala->ciudad, nombre);
	miSala->capacidad = capacidad;
	miSala->libres = capacidad;
	for(int i = 0; i < capacidad; i++) {
		miSala->asientos[i] = ASIENTO_LIBRE;
	}
	// fprintf(stderr, "Sala creada correctamente.\n");
	return capacidad;
}


int elimina_sala() {
	if (!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	
	free(miSala);
	miSala = NULL;
	// fprintf(stderr, "Sala eliminada correctamente.\n");
	return 0;
}


int existe_sala() {
	return miSala != NULL;
}


char * nombre_sala() {
	if (!existe_sala()) return NULL;
	return miSala->ciudad;
}


void estado_sala() {
    printf("Sala: %s\n", nombre_sala());
    printf("Capacidad: %i\n", capacidad_sala());
    printf("Asientos libres: %i\n", asientos_libres());
    printf("Asientos ocupados: %i\n", asientos_ocupados());
    for (int i = 0; i < capacidad_sala(); i++) {
        printf("[%*i] %-8i%s", digitos_asiento(), i, estado_asiento(i), (i+1)%5==0? "\n" : "");
    }
    printf("\n");
}

int digitos_asiento() {
    int digitos = 0;
    for (int capacidad = capacidad_sala(); capacidad > 0; capacidad /=10) digitos++;
    return digitos;
}

int guarda_estado_sala(const char * ruta, int existe) {
	if(!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	int fd;
	if (existe != 0) {
		fd = open(ruta, O_WRONLY | O_TRUNC);
	} else {
		fd = open(ruta, O_WRONLY | O_CREAT | O_EXCL, 0b111111111); 
	}
	if (fd == -1) {
		fprintf(stderr, "Error al abrir archivo: %s\n", strerror(errno));
		return -1;
	}
	
	if (write(fd, &miSala->ciudad, MAX_CIUDAD_LEN) == -1) {
		close(fd);
		fprintf(stderr, "Error al escribir (nombre): %s\n", strerror(errno));
		return -1;
	}
	
	
	if (write(fd, &miSala->capacidad, sizeof(int)) == -1) {
		close(fd);
		fprintf(stderr, "Error al escsribir (capacidad): %s\n", strerror(errno));
		return -1;
	}
	
	if (write(fd, &miSala->libres, sizeof(int)) == -1) {
		close(fd);
		fprintf(stderr, "Error al escribir (asientos libres): %s\n", strerror(errno));
		return -1;
	}
	
	for(int i = 0; i < capacidad_sala(); i++) {
		if (write(fd, &miSala->asientos[i], sizeof(int)) == -1) {
			close(fd);
			fprintf(stderr, "Error al escribir (asientos): %s\n", strerror(errno));
			return -1;
		}
	}
	
	close(fd);
	return 0;
}

int guarda_estado_parcial_sala(const char * ruta, size_t num_asientos, int* id_asientos) {
	if(!existe_sala()) {
		fprintf(stderr,"La sala no existe.\n");
		return -1;
	}
	
	int fd = open(ruta, O_WRONLY);
	if (fd == -1) {
		fprintf(stderr, "Error al abrir archivo: %s\n", strerror(errno));
		return -1;
	}
	
	char nombre_archivo[MAX_CIUDAD_LEN];
	if (read(fd, nombre_archivo, MAX_CIUDAD_LEN) != MAX_CIUDAD_LEN) {
		close(fd);
		fprintf(stderr, "Error al leer el nombre.\n");
		return -1;
	}
	
	int file_capacity;
	if (read(fd, &file_capacity, sizeof(int)) != sizeof(int)) {
		close(fd);
		fprintf(stderr, "Error al leer la capacidad del archivo.\n");
		return -1;
	}
	
	if (file_capacity != miSala->capacidad) {
		close(fd);
		fprintf(stderr, "Capacidad del archivo no coincide con la de la sala.\n");
		return -1;
	}
	
	
if (lseek(fd, MAX_CIUDAD_LEN + sizeof(int), SEEK_SET) == -1) {
		close(fd);
		fprintf(stderr, "Error al posicionar para escribir libres.\n");
		return -1;
	}
	if (write(fd, &miSala->libres, sizeof(int)) != sizeof(int)) {
		close(fd);
		fprintf(stderr, "Error al escribir número de libres.\n");
		return -1;
	}
	
	for (size_t i = 0; i < num_asientos; i++) {
		int id = id_asientos[i];
		if (id < 0 || id >= miSala->capacidad) {
			fprintf(stderr, "ID de asiento %d fuera de rango. Se ignora.\n", id);
			continue;
		}

		off_t offset = MAX_CIUDAD_LEN + sizeof(int) + sizeof(int) + id * sizeof(int);
		if (lseek(fd, offset, SEEK_SET) == -1) {
			close(fd);
			fprintf(stderr, "Error al posicionar el cursor para asiento %d.\n", id);
			return -1;
		}

		if (write(fd, &miSala->asientos[id], sizeof(int)) != sizeof(int)) {
			close(fd);
			fprintf(stderr, "Error al escribir estado de asiento %d.\n", id);
			return -1;
		}
	}
	
	close(fd);
	return 0;
}

int posicion_cursor_libres() {
	return MAX_CIUDAD_LEN*sizeof(char) + sizeof(int);
}

int posicion_cursor_asiento(int id_asiento) {
	return -1*(capacidad_sala()-id_asiento)*sizeof(int);
}

int recupera_estado_sala(const char* ruta) 
{
	int fd = open(ruta, O_RDONLY);
	if(fd == -1) 
	{
		fprintf(stderr, "Error al abrir el archivo\n", strerror(errno));
		return -1;
	}
	
	char name_file[MAX_CIUDAD_LEN];
	
	if(read(fd, &name_file, MAX_CIUDAD_LEN*sizeof(char)) != MAX_CIUDAD_LEN*sizeof(char))
	{
		close(fd);
		fprintf(stderr, "Error al leer el nombre");
		return -1;
	}
	int file_capacity;
	
	if(read(fd, &file_capacity, sizeof(int)) != sizeof(int))
	{
		close(fd);
		fprintf(stderr, "Error al leer la capacidad\n");
		return -1;
	}
	
	crea_sala(name_file, file_capacity);
	
	if(file_capacity != miSala-> capacidad)
	{
		close(fd);
		fprintf(stderr, "la capacidad del archivo\n");
		return -1;
	}
	
	if (read(fd, &miSala->libres, sizeof(int)) != sizeof(int)) {
		close(fd);
		fprintf(stderr, "Error al leer asientos libres.\n");
		return -1;
	}
	
	if(read(fd, &miSala->asientos, sizeof(int) * miSala->capacidad) != sizeof(int) * miSala->capacidad)
	{
		close(fd);
		fprintf(stderr, "error al leer los asientos\n");
		return -1;
	}
	
	close(fd);
	return 0;
}

int recupera_estado_parcial_sala(const char* ruta_fichero, size_t num_asientos, int* id_asientos) {
	if (!existe_sala()) {
		fprintf(stderr, "La sala no existe\n");
		return -1;
	}

	int fd = open(ruta_fichero, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Error al abrir el archivo\n");
		return -1;
	}

	char nombre_archivo[MAX_CIUDAD_LEN];
	if (read(fd, nombre_archivo, MAX_CIUDAD_LEN) != MAX_CIUDAD_LEN) {
		close(fd);
		fprintf(stderr, "Error al leer el nombre.\n");
		return -1;
	}

	int file_capacity;
	if (read(fd, &file_capacity, sizeof(int)) != sizeof(int)) {
		close(fd);
		fprintf(stderr, "Error al leer la capacidad.\n");
		return -1;
	}

	if (file_capacity != miSala->capacidad) {
		close(fd);
		fprintf(stderr, "La capacidad del archivo no coincide con la de la sala actual.\n");
		return -1;
	}

	if (lseek(fd, sizeof(int), SEEK_CUR) == -1) {
		close(fd);
		fprintf(stderr, "Error al saltar lectura de asientos libres.\n");
		return -1;
	}

	for (size_t i = 0; i < num_asientos; i++) {
		int id = id_asientos[i];

		if (id < 0 || id >= miSala->capacidad) {
			fprintf(stderr, "Asiento %d fuera de rango. Se omite.\n", id);
			continue;
		}

		off_t offset = MAX_CIUDAD_LEN + sizeof(int) + sizeof(int) + id * sizeof(int);
		if (lseek(fd, offset, SEEK_SET) == -1) {
			close(fd);
			fprintf(stderr, "Error en la posicion del asiento %d.\n", id);
			return -1;
		}

		if (read(fd, &miSala->asientos[id], sizeof(int)) != sizeof(int)) {
			close(fd);
			fprintf(stderr, "Error al leer el asiento %d.\n", id);
			return -1;
		}
	}

	int libres = 0;
	for (int i = 0; i < miSala->capacidad; i++) {
		if (miSala->asientos[i] == ASIENTO_LIBRE) {
			libres++;
		}
	}
	miSala->libres = libres;

	close(fd);
	return 0;
}


/** RETO**/
int anula(int id_persona) {
	if (!existe_sala()) return -1;

	int encontrado = 0;
	for (int i = 0; i < miSala->capacidad; i++) {
		if (miSala->asientos[i] == id_persona) {
			libera_asiento(i);
			encontrado = 1;
		}
	}
	if (!encontrado) return -1;
	return 0;
}

int compara(const char *ruta1, const char *ruta2) {
    struct stat st1, st2;

    if (stat(ruta1, &st1) == -1) {
        fprintf(stderr, "Error al obtener información de %s: %s\n", ruta1, strerror(errno));
        return -1;
    }

    if (stat(ruta2, &st2) == -1) {
        fprintf(stderr, "Error al obtener información de %s: %s\n", ruta2, strerror(errno));
        return -1;
    }

    if (st1.st_size != st2.st_size) {
        return 1; // Son diferentes por tamaño
    }

    int fd1 = open(ruta1, O_RDONLY);
    if (fd1 == -1) {
        fprintf(stderr, "Error al abrir %s: %s\n", ruta1, strerror(errno));
        return -1;
    }

    int fd2 = open(ruta2, O_RDONLY);
    if (fd2 == -1) {
        fprintf(stderr, "Error al abrir %s: %s\n", ruta2, strerror(errno));
        close(fd1);
        return -1;
    }

    char buf1[1024], buf2[1024];
    ssize_t r1, r2;
    int iguales = 1;

    while ((r1 = read(fd1, buf1, sizeof(buf1))) > 0 &&
           (r2 = read(fd2, buf2, sizeof(buf2))) > 0) {
        if (r1 != r2 || memcmp(buf1, buf2, r1) != 0) {
            iguales = 0;
            break;
        }
    }

    close(fd1);
    close(fd2);

    return iguales ? 0 : 1;
}

