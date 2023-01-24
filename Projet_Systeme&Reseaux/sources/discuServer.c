#include "../headers/discuServer.h"

#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/struct.h"

// la reponse du serveur au client
// fd est la socket
void discuServer(int fd, char *filename)
{

    char *buffer;

    int menu1;
    int menu2;

    struct Ligne ac;
    struct Lignes ac2;
    struct Lignes ac3;
    char *size;
    char *size2;
    size = (char *)malloc(1000);
    size2 = (char *)malloc(1000);
    // infinite loop for chat
    while (1)
    {
        // bzero(buffer, sizeof(buffer));
        read(fd, &menu1, sizeof(int));
        switch (menu1)
        {
        case 1:
            // recuperer les references
            read(fd, &menu2, sizeof(int));
            ac = req_reference(menu2, filename);
            // envoyer la taille du buffer souhaité
            write(fd, &ac.taille, sizeof(int));

            buffer = malloc(ac.taille);
            strcpy(buffer, ac.line);
            write(fd, buffer, ac.taille);
            bzero(buffer, ac.taille);
            break;
        case 2:

            // récupérer les mots
            bzero(size, 1000);
            read(fd, size, 1000);
            // printf("%s\n", size);
            ac2 = keyWord_request(size, filename);
            int a = ac2.nb_reponses;
            // envoyer le nombre de reponse
            write(fd, &a, sizeof(int));

            // envoie des lignes
            int i = 0;
            while (i < a)
            {
                write(fd, &ac2.taille[i], sizeof(int));
                buffer = malloc(ac2.taille[i]);

                strcpy(buffer, ac2.line[i]);
                write(fd, buffer, ac2.taille[i]);
                bzero(buffer, ac2.taille[i]);

                i++;
            }

            break;
        case 3:
            bzero(size, 1000);
            bzero(size2, 1000);

            // recuperer l'auteur
            read(fd, size, 1000);
            // recuperer le genre
            read(fd, size2, 1000);

            // Stocker le resultats dans une structure
            ac3 = auteurGenreRequete(size, size2, filename);

            // stocker le nombre de reponse
            int b = ac3.nb_reponses;
            // envoyer au client le nombre de reponse
            write(fd, &b, sizeof(int));
            int j = 0;
            while (j < b)
            {

                // envoyer la taille de la ligne
                write(fd, &ac3.taille[j], sizeof(int));
                buffer = malloc(ac3.taille[j]);

                // copier la ligne dans le buffer
                strcpy(buffer, ac3.line[j]);
                // envoyer la ligne
                write(fd, buffer, ac3.taille[j]);
                bzero(buffer, ac3.taille[j]);
                j++;
            }
            break;

        default:
            strcpy(buffer, "Choix introuvables , Tapez un nouveau choix ");
            write(fd, buffer, sizeof(buffer));
            break;
        }
    }
}
