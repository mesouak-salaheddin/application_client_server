#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
// inclusion des sockets
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../headers/req.h"
#include "../headers/discuServer.h"

// Fct pour fair le wait
void kill_child(int signal)
{
    // mort de la signal
    wait(&signal);
    printf("Mort du fils\n");
}

// fonction qui elimine les zombies en utilisant sigaction
void zombie_clean()
{
    struct sigaction ac;
    ac.sa_handler = kill_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);
}

// Noyau du serveur et l'appel a la fct pour la communication
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("Nombre d'arguments invalide \n");
        exit(-1);
    }
    char *filename = argv[2];

    int port = atoi(argv[1]);
    // char host[]="mouad-VirtualBox";
    // declaration et creation de la socket d'écoute
    int serveur_SOCKET = socket(AF_INET, SOCK_STREAM, 0);
    // char buffer[1024];

    if (serveur_SOCKET == -1)
    {
        perror("erreur lors de la creation de la socket d'écoute");
        exit(1);
    }

    printf("socket d'ouverture ouvert\n");

    // preparation des champs du struct sockaddr_in

    struct sockaddr_in adresse_serveur;

    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    adresse_serveur.sin_port = htons(port);

    // recupertaion de la taille de l'addresse serveur
    int taille = sizeof(adresse_serveur);

    // etablir la liason de la socket d'ecoute a l'adresse definie
    int link = bind(serveur_SOCKET, (struct sockaddr *)&adresse_serveur, taille);
    if (link == -1)
    {
        perror("impossible d'etablir la liaison (erreur en bind)");
        exit(2);
    }

    printf("liason etablie avec succès\n");

    // On ouvre le service en définissant un nombre limite de connexion simultanées
    int ecoute = listen(serveur_SOCKET, 5);
    if (ecoute == -1)
    {
        perror("erreur lors de l'ouverture du service");
        exit(3);
    }

    printf("service ouvert avec succès\n");
    printf("A l'écoute ...\n");

    // creation de l'adress client et et stockage de sa taille
    struct sockaddr_in adresse_client;
    socklen_t taille_adresse = sizeof(adresse_client);

    while (1)
    {
        printf("Le serveur attend une connection...\n");
        int client_SOCKET = accept(serveur_SOCKET, (struct sockaddr *)&adresse_client, &taille_adresse);
        if (client_SOCKET == -1)
        {
            perror("erreur lors de la creation de la socket client");
            exit(4);
        }
        printf("Connexion établie sur le port : %d, Identificateur du nouveau socket: %d \n", port, client_SOCKET);

        // creation du processus fils
        int child = fork();
        switch (child)
        {
        // gestion de l'erreur au cas ou la creation echoue
        case -1:
            perror("erreur de la creation du processu fils");
            exit(5);

        // comportement du processus fils
        case 0:
            printf("Le fils a été créé %d\n", getpid());
            close(serveur_SOCKET);
            // Appel a la fonction pour commencer la discussion
            discuServer(client_SOCKET, filename);
            close(client_SOCKET);
            exit(1);

        // comportement du processus pere
        default:
            // appelle a la fonction qui tue les zombies ( on sait pas encore si c est la bonne place pour appeler cette fct)
            zombie_clean();
            close(client_SOCKET);
            printf("Processus principal");
            break;
        }
    }

    return 0;
}