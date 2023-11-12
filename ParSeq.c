#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_ARG 100

struct noeud {
  int val; //1=seq 2=par 3=com
  char ** commande;
  struct noeud * fils;
  struct noeud *frere; 
};
typedef struct noeud *arbre;

 
char **Ligne2Argv(char*commande)
{
    char **arg;
    char *li;
    int i;
    char *p;
    li = strdup(commande); // strtok peut modifier ligne, on le met dans li. strdup alloue et copie
    arg = (char **)malloc(MAX_ARG * sizeof(char *)); // allocation de MAX_ARG pointeurs
    p = strtok(li," \t\n");
    i=0;
    while (p)
    {
        arg[i]=strdup(p);
        i++;
        p=strtok(NULL," \t\n");
    }
    arg[i]=NULL;
    return arg;
}

char *** file2tabArgv(char *f, int *nb)
{
    
    FILE * monFichier = fopen(f,"r");
    char *** tmp;
    tmp = (char***)malloc(256);
    int i;
    i = 0;
    char * uneLigne;
    uneLigne = (char *)malloc(256);
    if(monFichier != NULL)
    {
        printf("je suis dans le fichier\n");
        
        uneLigne = fgets(uneLigne, 256, monFichier);

        while(uneLigne != NULL)
        { 
            tmp[i] = Ligne2Argv(uneLigne);
            uneLigne = fgets(uneLigne, 256, monFichier);
            i++; 
        }
    }
    else
    {
        printf("je lis pas le fichier\n");
    }
    *nb = i;
    return tmp;
}
arbre creerFils(char *** tabArg, int * compteurCommande)
{
    
}
arbre creation(char *** tabArgv)
{
    arbre racine;
    int cpt;
    tabArgv = file2tabArgv("parSeq.txt", cpt);
    int compteurCommande;
    compteurCommande = 0;
    if(strcmp(tabArgv[compteurCommande][0], "seq") == 0)
    {
        racine = (racine) malloc(sizeof(struct noeud));
        racine->val = 1;
        racine->fils =creerFils(tabArgv, compteurCommande);
    }
    else if(strcmp(tabArgv[compteurCommande][0], "par") == 0)
    {
        racine = (racine) malloc(sizeof(struct noeud));
        racine->val = 2;
        racine->fils =creerFils(tabArgv, compteurCommande);
    }
    else
    {
        racine->val = 3;
        racine->commande = tabArg[compteurCommande];
        comcou++;
        racine->fils = NULL;
    }

}
int main()
{
    int cpt;
    char ***tabArgv;
    tabArgv = file2tabArgv("parSeq.txt", &cpt);
    printf("le nombre de commandes lu : %d \n", cpt);
    return 0;
}
