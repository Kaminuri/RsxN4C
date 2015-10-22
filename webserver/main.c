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

char * fgets_or_exit (char * buffer , int size , FILE * stream) {
	char * c;
	if ((c = fgets(buffer, size, stream)) == NULL)
		exit(1);

	return c;
}
void send_status(FILE * client , int code , const char * reason_phrase) {
	fprintf(client, "HTTP/1.1 %d %s \r\n", code, reason_phrase);	
}
int parse_http_request (const char * request_line , http_request * request) {
	char methode[10];
	char * url;
	url = malloc(256);
	if (url == NULL) {
		return -1;
	}
	printf("Recu : %s \n", request_line);
	if (sscanf(request_line, "%s %s HTTP/%d.%d", methode, url, &(*request).major_version, &(*request).minor_version) == 0){
		return -1;
	}
	request->url = url;
	printf("%d\n", strcmp(request->url, "/"));
	if (strcmp(methode, "GET") == 0){
		if (strcmp(request->url, "/") ==0){
			(*request).method = HTTP_GET;
		}
		else {
			(*request).method = HTTP_UNSUPPORTED;
			return 404;
		}
	} else {
		(*request).method = HTTP_UNSUPPORTED;
		return 400;
	}
	return 1;
}

void skip_headers( FILE * f) {
	char str[1024];
	while (1){
		fgets_or_exit(str, sizeof(str), f);
		if ((str[0] == '\r' && str[1] == '\n') || str[0] =='\n') break;
	} 
}


void send_response(FILE * client, int code, const char * reason_phrase, const char * message_body) {
	send_status(client, code, reason_phrase);
	int length = strlen(message_body);
	fprintf(client, "Connection: close \r\nContent-Length: %d \r\nContent-type: text/plain\r\n\r\n%s", length, message_body);
	fflush(client);
}



int main(){
	int taille=0;
	int socket_client, pid, erreur = 0;
	
	char str[1024];
	http_request req;
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
			fgets_or_exit(str, sizeof(str), f);
			skip_headers(f);
			erreur=parse_http_request(str,&req);
				
				//erreur = 1;
				/*strcpy(s, str);
				char* token = strtok(s, " ");
				while (token) {
					tokenCounter++;
					if(tokenCounter == 1 && !strcmp(token,"GET") == 0){
						erreur = 0;
					}
					if(tokenCounter ==2 && !strcmp(token, "/") ==0){
						erreur=0;
					}
					if(tokenCounter == 3 && (strncmp(token, "HTTP/1.1",8) != 0 && strncmp(token, "HTTP/1.0",8) !=0)){
						
						erreur = 0;
					}
					if(tokenCounter >=4){
						printf("%s\n", token);
					}			
					token = strtok(NULL, " ");

				}*/
			if(erreur == 404){
				send_response(f,404,"Page not found","Page not found\r\n");
			}else if(erreur == 400){
				send_response(f,400,"Bad Request","Bad Request\r\n");
			}else{
				fprintf(f, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\r\n", (int)strlen(message_bienvenue)+2, message_bienvenue);
			}

			fflush(f);
			fclose(f);
			
			
			/*while((taille = read(socket_client, buffer, 256)) != 0){
				if(write(socket_client, buffer, taille)== -1){
					perror("Erreur write retour de message");
				}
			}*/
			exit(EXIT_SUCCESS);
		}else{
			close(socket_client);
		}

	}
	return 1;
}
