#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sala.c>


void crea_sucursal(const char * ciudad, int capacidad)
{
	pid_t pid;
	pid = fork();
	
	switch (pid)
	{
		case -1:
			perror("fork");
		case 0:
			exec(char* args[] = {"ls", "-l", "NULL"};);
		default:
			
	}	
}
