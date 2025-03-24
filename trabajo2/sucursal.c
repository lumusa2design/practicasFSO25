#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



void crea_sucursal(const char * ciudad,const char * capacidad)
{
	pid_t pid;
	pid = fork();
	
	switch (pid)
	{
		case -1:
			perror("fork");
		case 0:
			char* args[] = {"xterm","NULL"};
			printf("Soy el hijo\n");
			execlp("gnome-terminal","gnome-terminal",NULL);
		default:
			printf("Soy el padre\n");
			
	}
	
	
}

int main()
{
	crea_sucursal("hola", "5");
	return 0;
}
