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

typedef struct noeud {
  int val; //1=seq 2=par 3=com
  char ** commande;
  struct noeud * fils;
  struct noeud *frere;
} arbre;
arbre *a;


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
void AfficheArgv(char** argument)
{
    int i=0;
    while(argument[i])
    {
        printf("argv %d : %s ", i, argument[i]);
        i++;
    }
    printf("\n");
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
        printf("########## ouverture du fichier ########## \n");
        printf("##########################################\n");
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
int Executer(char** chaine) {

    int pid, res, ret;
    pid = fork();

    if (pid == 0) { // le fils
        
        sleep(2);
        execvp(chaine[0], chaine);
        exit(3);
    }

    else if ( pid > 0) { // le pere
        
        res = wait(&ret);
      
    }
    
    else {
        fprintf(stderr,"PERE: pb, je n'ai pas pu creer mon fils\n");
        return 255;
    }
}

int renisComCou = 0;
arbre * creation(char *** tabArgv, char * file);
arbre * creerFils(char *** tabArgv, int comCou)
{
    
    arbre  *c, * cPrec;
    cPrec = NULL;
    arbre * cPremier;
    
    while(tabArgv[comCou] != NULL && (strcmp(tabArgv[comCou][0], "endpar") != 0 && strcmp(tabArgv[comCou][0], "endseq") != 0))
    {
        
        c = malloc(sizeof (arbre));
        if(cPrec != NULL)
        {
            
            cPrec->frere = c;
        }
        else
        {
            cPremier = c;
        }
        if(strcmp(tabArgv[comCou][0],"seq") == 0)
        {
            
            c->val = 1;
            comCou++;
            //printf("comcou avant appel : %d \n ", comCou);
            c->fils = creerFils(tabArgv, comCou);
            //printf("comcou apres appel : %d \n ", comCou);
            //renisComCou = renisComCou-1;
            comCou = renisComCou;
            //printf("dans seq renis comcou : %d \n ", comCou);
            
            
        }
        else if(strcmp(tabArgv[comCou][0],"par") ==0)
        {
            c->val = 2;
            comCou++;
            c->fils = creerFils(tabArgv, comCou);
            //renisComCou = renisComCou-1;
            comCou = renisComCou;
            //printf("dans par renis comcou : %d \n ", comCou);
        }
        else
        {
            c->val = 3;
            c->commande = tabArgv[comCou];
            comCou++;
            
        }
    cPrec = c;
    }
    comCou++;
    c->frere = NULL;
    //printf("comcou avant retour : %d \n", comCou);
    renisComCou = comCou;
    return cPremier;
}

void execSeq(arbre * p);
void execPar(arbre * p);

void execParSeq(arbre * p) // l arbre racine
{
    if(p->val == 1)
    {
        
        execSeq(p->fils);
    }
    else if(p->val == 2)
    {
        
        execPar(p->fils);
    }

    else
    {
        Executer(p->commande);
    }
}

void aff(arbre * r)
{
    if(r == NULL)
    {
        return;
    }
    printf("%d \n",r->val);
    aff(r->fils);
    aff(r->frere);
    
}
int main(int argc, char ** argv)
{
    arbre * r;
    char *** tab;
    r = creation(tab, argv[1]);
    /*printf("affichage de l'arbre : \n");
    aff(r);*/
    execParSeq(r);
    
    return 0;
}
void execSeq(arbre * p)
{
    int res;
    int status;
    while(p)
    {
        res = fork();
        if(res==0)
        {
            execParSeq(p);
            exit(3);
        }
        else
        {
            wait(&status);
        }
        p = p->frere;
    }
}
arbre * creation(char *** tabArgv, char * file)
{
    arbre *racine;
    int cpt;
    tabArgv = file2tabArgv(file, &cpt);
    int compteurCommande;
    compteurCommande = 0;
    racine = (arbre*)malloc(sizeof(struct noeud));
    if(strcmp(tabArgv[compteurCommande][0], "seq") == 0)
    {
        
        racine->val = 1;
        racine->commande = tabArgv[compteurCommande];
         compteurCommande++;
        racine->fils = creerFils(tabArgv, compteurCommande);
        racine->frere = NULL;
    }
    else if(strcmp(tabArgv[compteurCommande][0], "par") == 0)
    {
        
        racine->val = 2;
        racine->commande = tabArgv[compteurCommande];
        compteurCommande++;
        racine->fils = creerFils(tabArgv, compteurCommande);
        racine->frere = NULL;
    }
    else
    {
        racine->val = 3;
        racine->commande = tabArgv[compteurCommande];
        racine->fils = NULL;
        racine->frere = NULL;
    }
    return racine;
}
void execPar(arbre * p)
{
    int temp = 0;
    int res1, res2;
    int status1, status2;
  
        res1 = fork();
        if(res1 == 0)
        {
            
            while(p)
            {
                res2 = fork();
                if(res2 == 0)
                {
                    execParSeq(p);
                    exit(3);
                }
                else
                {
                    temp++;
                  //wait en dehors de la boucle 
                }
                p = p->frere;
            }
            
            for(int i =0; i < temp; i++)
            {
                wait(&status2);
            }
        }
        else
        {
            wait(&status1);
        }
        
    }

