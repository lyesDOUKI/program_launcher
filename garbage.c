///////////// creation arbre
ENREXEC *CreerArbre(char ***tabArg)
{
    ENREXEC *c;
    int comcou = 0;
    c = malloc(sizeof (ENREXEC));
    if(strcmp(tabArg[comcou][0], "SEQ") == 0)
    {
        c->type = 'S';
        c->commande = tabArg[comcou];
        comcou++;
        c->fils = CreerFils(tabArg, comcou);
        c->frere = NULL;
    }
    else if(strcmp(tabArg[comcou][0], "PAR") == 0)
    {
        c->type = 'P';
        c->commande = tabArg[comcou];
        comcou++;
        c->fils = CreerArbre(tabArg);
        c->frere = CreerArbre(tabArg);
    }
    else
    {
        c->type = 'C';
        c->commande = tabArg[comcou];
        comcou++;
        c->fils = NULL;
        c->frere = CreerArbre(tabArg);
    }
    
}
//////////////////////creation fils 
noeud * creerFils(char *** tabArgv, int * comCou)
{
    noeud  *c, * cPrec;
    cPrec = NULL;
    noeud * cPremier;
    while(tabArgv[comCou] != NULL && strcmp(tabArgv[comCou][0] != "endpar") && strcmp(tabArgv[comCou][0] != "endseq"))
    {
        c = new(noeud);
        if(cPrec != NULL)
        {
            cPrec->frere = c;
        }
        else
        {
            cPremier = c;
        }
        if(strcmp(tabArgv[comCou][0],"par") ==0)
        {
            c->type = "P";
            comCou++;
            c->creerFils(tabArgv, comCou)
        }
        else if(strcmp(tabArgv[comCou][0],"seq") ==0)
        {
            c->type = "S";
            comCou++;
            c->creerFils(tabArgv, comCou)
        }
        else
        {
            c->type = "C";
            c->commande = tabArgv[comCou][0];
            comCou++
        }
    cPrec = c;
    }
    comCou++;
    c->frere = null;
}
//////////////////ParExcec
execParSeq(arbre * p) // l arbre racine
{
 if(p->type == "P")
 {
 execPar(p->fils);
 }
 else if(p->type == "S")
 {
 execSeq(p->fils);
 }
 else
 {
 execute(p->commande);
 }
}

execSeq(arbre * p)
{
 while(p)
 {
 res = fork()
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

execPar(arbre * p)
{
 faire 2 fork
 1 fork 
 {
 son fils fait un fork
 le 2 eme fork
 {
 execute les commande
 }
 {
 le pere du deuxieme : attend toutes les commande // voir exo 7 tp1
 }
 }
 le pere du fork 1 fait 1 wait 
}