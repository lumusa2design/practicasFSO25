#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sala.c>


void crea_sucursal(const char * ciudad, int)
{
	pid_t pid;
	pid = fork();
	
	switch (pid)
	{
		case -1:
			perror("fork");
	}	
}
