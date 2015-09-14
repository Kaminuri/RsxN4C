#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h> 
#include <unistd.h>
#include "socket.h"
int main(){
	int socket_client, pid;
	char buffer[256];
	const char *message_bienvenue = "Bonjour, c'est un grand honneur de vous avoir sur notre serveur. Bienvenue à bord !\n";
	int socket_serveur = creer_serveur(8080);
	message_bienvenue +=0;

	while((socket_client = accept(socket_serveur , NULL , NULL)) != -1){
		if((pid = fork()) == -1){
			perror("Erreur fork");
		/* traitement d’erreur */
		}

		if(pid == 0){
			sleep(1);
			if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) == -1){
				perror("Erreur write bienvenu");
		/* traitement d’erreur */
			}
			while(read(socket_client, buffer, 256) != -1){
				if(write(socket_client, buffer, 256)== -1){
					perror("Erreur write retour");
				/* traitement d’erreur */
				}
			}
		}
		wait(0);
	}
	return 1;
}