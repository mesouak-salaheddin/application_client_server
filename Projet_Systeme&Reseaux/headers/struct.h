#define SIZE 50
typedef struct Ligne
{
    char *line;
    int taille;

} Ligne;
Ligne req_reference(int ref,char* filename );
typedef struct Lignes
{
    char line[SIZE][500];
    int taille[SIZE];
    int nb_reponses;
} Lignes;
char *titleCheck(char *ligne);
char *authorCheck(char *ligne);
char *genderCheck(char *ligne);
char *ligneParMot(char *mot , char* filename);
int tailleParMot(char *mot,char* filename);
Lignes keyWord_request(char *liste_mots,char* filename);
Lignes auteurGenreRequete(char *auteur, char *genre, char* filename);
