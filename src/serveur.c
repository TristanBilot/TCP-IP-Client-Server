/*
	@file server.c
    @author Tristan Bilot
*/

#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include "data.h"

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server, client;
	char client_message[MSG_SIZE], cpt_clients_string[3]; // pour affichage avec puts;
    int cpt_clients = 0;
    int allumé = 1;
	
	//Création de la socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Erreur de création de la socket");
	}
	puts("Socket creee");
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( IPPORT_USERRESERVED );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Erreur bind");
		return 1;
	}
	puts("Le bind est un succès !");
	
	//Mode écoute
	listen(socket_desc , 3);
	
	puts("Serveur en attente de connexions entrantes...\n");
	c = sizeof(struct sockaddr_in);
	
	//Le fork permet d'accepter plusieurs clients (1 processus par client)
int ret = fork();
if (ret == 0) {
    //Le serveur boucle pour renouveler les connexions entrantes
while(allumé) {// FILS
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0) {
            perror("Erreur accept");
            return 1;
        }
        puts("Connection acceptee !");
         
        //Réception du msg du client
        while( (read_size = recv(client_sock , client_message , MAX_READ , 0)) > 0 ) {
            //Envoi du msg au client
            puts("[+] Message du client: ");
            puts(client_message);
            write(client_sock , client_message , strlen(client_message));
        }
                    
        if(read_size == 0) {
            puts("Client deconnecte");
            fflush(stdout);
        }
        else if(read_size == -1) {
            perror("Erreur recv");
        }        
}
	} 
    else 
    {

while (allumé) { // PERE
        
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("Erreur accept");
            return 1;
        }
        puts("Connection acceptee !");

        //Réception du msg du client
        while( (read_size = recv(client_sock , client_message , MAX_READ , 0)) > 0 ) {
            //Réception et affichage du msg client
            client_message[read_size] = '\0';
            puts("[+] Message du client: ");
            puts(client_message);
            write(client_sock , client_message , strlen(client_message));
        }
                    
        if(read_size == 0) {
            puts("Client deconnecte");
            fflush(stdout);
        }
        else if(read_size == -1) {
            perror("Erreur recv");
        }
        memset(client_message,'0', read_size);
		
}
    }
	
	return 0;
}