#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"

int creer_serveur(int port){
	int socket_serveur;
	int socket_client, pid;
	const char *message_bienvenue = "Bonjour, c'est un grand honneur de vous avoir sur notre serveur. Bienvenue à bord !\n";
	socket_serveur = socket(AF_INET , SOCK_STREAM , 0);
	message_bienvenue +=0;


	
	if (socket_serveur == -1){
		perror("socket_serveur");
		/* traitement de l’erreur */
	}
	/* Utilisation de la socket serveur */
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	/* Socket ipv4 */
	saddr.sin_port = htons(port);
	/* Port d’écoute */
	saddr.sin_addr.s_addr = INADDR_ANY;
	/* écoute sur toutes les interfaces */
	if (bind(socket_serveur , (struct sockaddr *)&saddr , sizeof(saddr)) == -1){
		perror("bind socker_serveur");
		/* traitement de l’erreur */
	}
	if (listen(socket_serveur , 10) == -1){
		perror("listen socket_serveur");
		/* traitement d’erreur */
	}
	
	while((socket_client = accept(socket_serveur , NULL , NULL)) != -1){
		pid = fork();
		if(pid == 0){
			sleep(1);
			write(socket_client, message_bienvenue, strlen(message_bienvenue));
		}
	}
	return socket_serveur;
}