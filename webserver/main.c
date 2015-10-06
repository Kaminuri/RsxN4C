#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include "socket.h"
int main(){
	int taille=0;
	int socket_client, pid, tokenCounter = 0, pageOK = 1;
	char s[256];
	char str[1024];
	FILE *f = NULL;
	const char *message_bienvenue = "Bonjour, c'est un grand honneur de vous avoir sur notre serveur. Bienvenue à bord !\n";
	int socket_serveur = creer_serveur(8080);
	message_bienvenue +=0;
	taille+=0;
	
	initialiser_signaux();
	while((socket_client = accept(socket_serveur , NULL , NULL)) != -1){
		
		if((pid = fork()) == -1){
			perror("Erreur fork");
		/* traitement d’erreur */
		}
		if(pid == 0){
			sleep(1);
			f = fdopen(socket_client, "w+");
			/* if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) == -1){
				perror("Erreur write bienvenu");
				traitement d’erreur 
			}*/
			
			if(fgets(str, sizeof(str), f)) {
				strcpy(s, str);
				char* token = strtok(s, " ");
				while (token) {
					tokenCounter++;
					if(!(tokenCounter == 1 && strcmp(token,"GET") == 0)){
						printf("TOKEN 1\n");
						pageOK = 0;
					}
					
					if(!(tokenCounter == 3 && ( strcmp(token, "HTTP/1.1\r\n") == 0 || strcmp(token, "HTTP/1.0\r\n") == 0 ))){
						printf("TOKEN 2\n");
						pageOK = 0;
					}
					token = strtok(NULL, " ");
				}
				if(pageOK == 0){
					fprintf(f, "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request\r\n");
				}
			}
			/*while((taille = read(socket_client, buffer, 256)) != 0){
				if(write(socket_client, buffer, taille)== -1){
					perror("Erreur write retour de message");
				}
			}*/
			if(taille == -1){
				perror("Erreur read retour");
			}
			exit(EXIT_SUCCESS);
		}else{
			close(socket_client);
		}

	}
	return 1;
}