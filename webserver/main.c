#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
int main(){
	int socket_serveur = creer_serveur(8080);
	return 1;
}