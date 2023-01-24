#include "../headers/discuClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../headers/req.h"

void menu()
{
    printf("---------------------------------------MENU----------------------------------------\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf("\nBienvenue dans le server \n 1->livre depuis sa reference   \n 2->Livre par mots clés (ils doivent être dans le titre )  \n 3->Savoir la reference et le titre depuis l'auteur et le genre \n 4-> Tapez 5 pour sortir \n");
}

// fct pour la communication avec le serveur et l'utilisateur
// fs est la socket
void discuClient(int fs)
{
    /*     char *buffer;
        buffer = (char *) malloc(sizeof(char *)); */
    char *buffer;
    int choix;
    int taille;
    char *mot;
    char *mot2;
    mot = (char *)malloc(1000);
    mot2 = (char *)malloc(1000);
    int choix2;
    int n = 0;
    int nb_reponse;
    int nb_reponse2;
    int test;
    int tmp;

    // la sortie de cette boucle sera l'écriture de STOP en majuscule
    while (n == 0)
    {

        printf("\nQuel requête voulez vous le faire \n (Taper 5 pour sortir) \n");
        printf("choix requete : ");

        // recuperer le choix de l'utilisateur
        test = scanf("%d", &choix);

        // Si l'utilisateur a rentré un charactere
        while (test != 1)
        {
            while ((tmp = getchar()) != EOF && tmp != '\n')
                ;
            printf("Vous avez Tapez un character , Tapez un nombre de 1 a 5 SVP :)  : ");
            test = scanf("%d", &choix);
        }

        // Tester si l'utilisateur a entré un choix different de 1 /2 /3 /5 , et apres si il a entré un char
        while (((choix != 1) && (choix != 2) && (choix != 3) && (choix != 4) && (choix != 5)) || (test != 1))
        {
            while ((tmp = getchar()) != EOF && tmp != '\n')
                ;
            printf("Choix introuvable , Tapez un nouveau choix \n Ou bien (Taper 5 pour sortir)\n");
            printf("choix: ");
            test = scanf("%d", &choix);
        }
        printf("\n");

        // envoyer au serveur le choix
        write(fs, &choix, sizeof(int));

        switch (choix)
        {

        case 1:
            // Demander au utilisateur la reference
            printf("\tQuelle votre reference: ");
            scanf("%d", &choix2);
            printf("\n");

            // envoyer cette reference au serveur
            write(fs, &choix2, sizeof(int));

            // recuperer la taille de la ligne souhaité
            read(fs, &taille, sizeof(int));

            buffer = malloc(taille);
            read(fs, buffer, taille);
            // read(fs,buffer,sizeof(buffer));

            // afficher la reponse
            aff_ref(buffer);
            break;

        case 2:

            // Demander au utilisateur la reference
            bzero(mot, 1000);
            printf("Tapez vos mots : ( Oubliez pas la majuscule svp ) : ");

            // On a utiliser ce scanf pour recuperer toute la chaine de charactere meme apres un espace
            scanf(" %[^\n]s", mot);

            // envoyer cet mots au serveur
            write(fs, mot, 1000);

            // recuperer le nombre de reponses du serveur
            read(fs, &nb_reponse, sizeof(int));
            int i = 0;
            while (i < nb_reponse)
            {

                // lire la taille
                read(fs, &taille, sizeof(int));

                // creer un buffer de cette taille et allouer de la memoire
                buffer = malloc(taille);

                // recuperer la ligne
                read(fs, buffer, taille);

                // Organiser l'affichage
                affichage2(buffer);
                bzero(buffer, taille);
                i++;
            }
            break;
        case 3:

            // On a utiliser ce scanf pour recuperer toute la chaine de charactere meme apres un espace
            printf("Quel est auteur : ( Oubliez pas la majuscule svp ) : ");
            scanf(" %[^\n]s", mot);
            // envoyer l'auteur
            write(fs, mot, 1000);

            printf("Quel est le genre :");
            scanf(" %[^\n]s", mot2);
            // envoyer le genre
            write(fs, mot2, 1000);

            // recuperer le nombre de reponse
            read(fs, &nb_reponse2, sizeof(int));
            int j = 0;
            while (j < nb_reponse2)
            {

                // recuperer la taille de la ligne
                read(fs, &taille, sizeof(int));

                buffer = malloc(taille);

                // recuperer la ligne
                read(fs, buffer, taille);

                // afficher la ligne
                affichage3(buffer);
                bzero(buffer, taille);
                j++;
            }
            break;
        case 5:
            // si l'utilsateur a tapé 5
            printf("Merci pour votre visite , a bientot  \n");
            n++;
            break;
        }
    }
}