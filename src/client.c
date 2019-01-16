/*
	@file client.c
    @author Tristan Bilot
*/

#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include "data.h"

int main(int argc , char *argv[])
{
	int sock, read_size;
	struct sockaddr_in server;
	char message[MSG_SIZE] , server_reply[MSG_SIZE];
    int run = 1;
	
	// Création de la socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Erreur de création de la socket");
	}
	puts("Socket creee");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
	server.sin_family = AF_INET;
	server.sin_port = htons( IPPORT_USERRESERVED ); // port 5000

	// Connexion au serveur
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Erreur connect");
		return 1;
	}
	
	puts("Connecte !\n");
	
	// Boucle pour envoyer plusieurs messages
	while(run) {		
		printf("Entrez le message : ");
		scanf("%s" , message);
		
		// Envoi du message au serveur
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Erreur envoi");
			return 1;
		}

		read_size = recv(sock , server_reply , MAX_READ , 0);
        message[read_size] = '\0';

        //Envoi du msg au serveur
        puts("[+] Message du serveur : ");
        puts(server_reply);

    	memset(message,'0', read_size);
        
	}
	
	close(sock);
	return 0;
}