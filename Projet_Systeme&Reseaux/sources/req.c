#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <aio.h>
#include "../headers/struct.h"

//fonction qui retourne une structure ligne qui contient la ligne qui correspond a la ref et sa taille
Ligne req_reference(int ref, char * filename)
{
    struct Ligne res;
    res.line = (char *)malloc(100*sizeof(char));
    //res.taille= (int)malloc(500*sizeof(int)); 
    if(res.line == NULL){
        printf("allocation de momoirre echoue\n");
    }
    char *mot;
    FILE *fichier;
    char *ligne = (char *)malloc(1000);
    size_t len = 0;
    ssize_t read;
    fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("fichier inexistant \n");
    }
    int refTMP;
    res.taille = 0;
    char tmp[1024];
    //lecture de chaque line du fichier
    while ((read = getline(&ligne, &len, fichier)) > 1)
    {

        strcpy(tmp,ligne);
        mot = strtok(ligne, "#");
        refTMP = atoi(mot);
        if (ref == refTMP)
        {
            //on stocke la ligne voulu dans la structure
            strcpy(res.line,tmp);
            //on stock la taille de la ligne lue dans la structure
            res.taille = read;
        }

    }

    fclose(fichier);

    if(res.taille==0)
    {
        strcpy(res.line, "reference inexistante\n");
        res.taille = sizeof("reference inexistante\n");
    }
    return res;
}


//fonction qui prend une ligne et retourne le titre dans cette ligne
char *titleCheck(char *ligne){
    char *mot;
    mot = (char *)malloc(1000);
    char *res;
    res = (char *)malloc(1000);
    mot = strtok(ligne, "#");
    for(int i = 1; i < 6;i++){
        mot = strtok(NULL, "#");
        switch (i){
        case 2:
            // titre
            strcpy(res,mot);
            break;
        default:
            break;
        }
    }
    return res;
}

//fonction qui prend une ligne et retourne l'auteur dans cette ligne
char *authorCheck(char *ligne){
    char *mot;
    mot = (char *)malloc(1000);
    char *res;
    res = (char *)malloc(1000);
    mot = strtok(ligne, "#");
    for(int i = 1; i < 6;i++){
        mot = strtok(NULL, "#");
        switch (i){
        case 1:
            // titre
            strcpy(res,mot);
            break;
        default:
            break;
        }
    }
    return res;
}

//fonction qui prend une ligne et retourne le genre dans cette ligne
char *genderCheck(char *ligne){
    char *mot;
    mot = (char *)malloc(1000);
    char *res;
    res = (char *)malloc(1000);
    mot = strtok(ligne, "#");
    for(int i = 1; i < 6;i++){
        mot = strtok(NULL, "#");
        switch (i){
        case 3:
            // titre
            strcpy(res,mot);
            break;
        default:
            break;
        }
    }
    return res;
}

//fonction qui renvoie la ligne où le titre contient le mot cle passer en parametre
char *ligneParMot(char *mot , char* filename){
    FILE *fichier;
    char *ligne;
    ligne = malloc(1000);
    size_t len = 0;
    ssize_t read;
    char *result = (char *) malloc(1000*sizeof(char));
    //ouverture du fichier pour chaque recherche
    fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("fichier inexistant \n");
    }
    bool trouve = false;
    char *titre = (char *)malloc(1000*sizeof(char));
    char *store = (char *)malloc(1000*sizeof(char));

    //on recupere le premier element du tableau et on procede a la recherche
    while ((read  = getline(&ligne, &len, fichier))> 0){
        strcpy(store,ligne);
        strcpy(titre,titleCheck(ligne));
        if (strstr(titre, mot) != NULL){
            trouve =true;
            strcpy(result,store);

        }
    }
    if(!trouve){
        strcpy(result,"echec");

    }
    fclose(fichier);
    return result;
}
//fonction qui renvoie la taille de la ligne où le titre contient le mot cle passer en parametre
int tailleParMot(char *mot, char *filename){
    FILE *fichier;
    char *ligne = (char *)malloc(1000);
    size_t len = 0;
    ssize_t read;
    int result ;
    //ouverture du fichier pour chaque recherche
    fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("fichier inexistant \n");
    }
    bool trouve = false;
    //on recupere le premier element du tableau et on procede a la recherche
    char *titre = (char *)malloc(1000*sizeof(char));
    char *store = (char *)malloc(1000*sizeof(char));
    while ((read  = getline(&ligne, &len, fichier))> 0){
        strcpy(store,ligne);
        strcpy(titre,titleCheck(ligne));
        if (strstr(store, mot) != NULL){
            trouve =true;
            result = read;
        }
    }
    if(!trouve){
        result = sizeof("aucun des mots clé n'existe");
    }
    fclose(fichier);
    return result;
}

/*renvoie une structure qui contient une liste de ligne où on a stocker les lignes correspondantes
aux mots clés et leur taille respective et le nombre de reponse trouvé*/   
Lignes keyWord_request(char *liste_mots ,char *filename){
    struct Lignes resultat;
    //creation d'un tableau de mots cles
    char *liste_m = strdup(liste_mots);
    char *cle;
    int j = 0;
    char list[20][50];
    cle = strtok(liste_m, " ");
    char *keyWords;
    resultat.nb_reponses = 0;


    while (cle && strcmp(cle, "\n") != 0)
    {
        strcpy(list[j], cle);
        cle = strtok(NULL, " ");
        resultat.nb_reponses++;
        j++;
    }
    //pour chaque mot on va recuperer les lignes corresponantes
    int t = 0;

    while (t < 5 && (list[t] != NULL)){   
        keyWords = list[t];
        if(strcmp(ligneParMot(keyWords,filename),"echec") == 0){
            strcpy(resultat.line[t],"aucun des mots clé n'existe");
            resultat.taille[t] = sizeof("aucun des mots clé n'existe");
        }else{
            strcpy(resultat.line[t],ligneParMot(keyWords,filename));
            resultat.taille[t] = tailleParMot(keyWords,filename);

        }
        t++;
    }

    return resultat;
}


// Renvoi une structure qui contient une liste de lignes et leurs tailles et le nombre de reponses où l'auteur et le genre correspondent au parametres donnés
Lignes auteurGenreRequete(char *auteur, char *genre, char* filename){
    struct Lignes resultats;
    resultats.nb_reponses = 0;
    FILE *fichier;
    char *ligne = NULL;
    size_t len = 0;
    ssize_t read;
    fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("fichier inexistant \n");
    }
    int i = 0;
    bool trouve = false;
    char *author = (char *)malloc(1000*sizeof(char));
    char *gender = (char *)malloc(1000*sizeof(char));
    char *store1 = (char *)malloc(1000*sizeof(char));
    char *store2 = (char *)malloc(1000*sizeof(char));
    
    while ((read = getline(&ligne, &len, fichier)) > 1)
    {   
        strcpy(store1,ligne);
        strcpy(store2,ligne);
        strcpy(author,authorCheck(ligne));
        strcpy(gender,genderCheck(store1));
        if ((strstr(author, auteur) != NULL)&&(strstr(gender, genre) != NULL))
        {
            trouve =true;
            strcpy(resultats.line[i],store2);
            resultats.taille[i] = read;
            i++;
        }
    }
    resultats.nb_reponses = i;
    if(!trouve){
        strcpy(resultats.line[i],"aucun resultat trouve pour cette combinaison");
        resultats.taille[i]= sizeof("aucun resultat trouve pour cette combinaison");
        resultats.nb_reponses = 1;
    }
    return resultats;
}

//Affichage et formatage de la requete 3 par le client 
void affichage3(char *line)
{
    if (strcmp(line, "aucun resultat trouve pour cette combinaison") == 0)
    {
        printf("\n%s\n", line);
    }

    else
    {
        char *mot;
        int i;

        mot = strtok(line, "#");

        printf("\n--------------------  Informations  ------------------\n\nreference : %s\n\n", mot);
        
        i = 1;

        while (mot && strcmp(mot, "\n") != 0)
        {
            if (i > 3)
            {
                break;
            }
            mot = strtok(NULL, "#");
            switch (i)
            {

            case 2:
                // titre
                // mot = strtok(NULL, "#");
                printf("titre : %s\n", mot);
                i++;
                break;

            
            default:
                i++;
                break;
            }

            printf("\n");
        }
        printf("------------------------------------------------------\n");
    }
}
//le client format les données de la reference et les affichent  à l'utilisateur 
void aff_ref(char *line)
{
    if (strcmp(line, "reference inexistante\n") == 0)
    {
        printf("%s", line);
    }

    else
    {
        char *mot;
        int i;
        int nb_pages;
        mot = strtok(line, "#");

        printf("%s", mot);
        i = 1;
        nb_pages = 0;
        while (mot && strcmp(mot, "\n") != 0)
        {
            if (i > 4)
            {
                break;
            }
            mot = strtok(NULL, "#");
            switch (i)
            {
            // Remplissage du tableau
            case 1:
                // auteur
                // mot = strtok(NULL, "#");
                printf("Auteur : %s\n", mot);
                i++;
                break;
            case 2:
                // titre
                // mot = strtok(NULL, "#");
                printf("titre de : %s\n", mot);
                i++;
                break;
            case 3:
                // genre
                // mot = strtok(NULL, "#");
                printf("genre :%s \n", mot);
                i++;
                break;
            case 4:
                // nbt de pages
                // mot = strtok(NULL, "#");
                nb_pages = atoi(mot);
                if (nb_pages > 300)
                {
                    printf("nombre de page : superieur à 300 \n");
                }
                else
                {
                    printf("nombre de page : inférieur à 300 \n");
                }
                i++;
                break;
            default:
                i++;
                break;
            }

            printf("\n");
        }
    }
}

//le client format les données de la requete 2 et les affichent pour l'utilisateur 
void affichage2(char *line)
{
    if (strcmp(line, "aucun des mots clÃ© n'existe") == 0)
    {
        printf("%s", line);
    }

    else
    {
        char *mot;
        int i;

        mot = strtok(line, "#");

        printf("\n------------------------------------------------------\n\nreference : %s\n\n", mot);
        i = 1;

        while (mot && strcmp(mot, "\n") != 0)
        {
            if (i > 3)
            {
                break;
            }
            mot = strtok(NULL, "#");
            switch (i)
            {
            // Remplissage du tableau
            case 1:
                // auteur
                // mot = strtok(NULL, "#");
                printf("Auteur : %s\n", mot);
                i++;
                break;
            case 2:
                // titre
                // mot = strtok(NULL, "#");
                printf("titre : %s\n", mot);
                i++;
                break;
            case 3:
                // genre
                // mot = strtok(NULL, "#");
                printf("genre : %s \n", mot);
                i++;
                break;
            
            default:
                i++;
                break;
            }

            printf("\n");
        }
        printf("------------------------------------------------------\n");
    }
}