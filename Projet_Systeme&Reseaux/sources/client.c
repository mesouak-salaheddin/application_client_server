#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../headers/discuClient.h"
#include "../headers/req.h"
#include "../headers/struct.h"

// Ce main de client contient que le noyau , la communicateur se fait via la fonction discuClient

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        perror("Nombre d'arguments invalide \n");
        exit(-1);
    }

    // Declaration du Host et du Port
    int port = atoi(argv[2]);
    char *host = argv[1];

    // Declaration des arguments
    struct sockaddr_in client_sockaddr;
    int client_socket;
    struct hostent *serveur_name;
    char *h_addr;

    // Hostname
    serveur_name = gethostbyname(host);
    if (serveur_name == NULL)
    {
        perror("Erreur de gethostname\n");
        exit(-1);
    }

    // Creation de la socket pour client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Erreur de Socket");
        exit(1);
    }
    printf("TCP server a été bien crée.\n");

    // Affectation des arguments
    memcpy(&client_sockaddr.sin_addr, serveur_name->h_addr, sizeof(u_long));
    client_sockaddr.sin_family = AF_INET;
    client_sockaddr.sin_port = (htons(port));

    // Connection du client sur le serveur
    int conex = connect(client_socket, (struct sockaddr *)&client_sockaddr, sizeof(client_sockaddr));

    if (conex < 0)
    {
        perror("Erreur de connection du client");
        exit(3);
    }

    printf("Le client est connecté au serveur.\n");

    // Affichage du menu
    menu();

    // Commencer la discussion avec le serveur
    discuClient(client_socket);

    close(client_socket);
    printf("Le client a quitté le serveur .\n");

    return 0;
}