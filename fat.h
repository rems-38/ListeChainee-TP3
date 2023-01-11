#define BLOCSIZE 512
#define BLOCNUM 1024
#define NAMELEN 256

struct objet {
    char nom[NAMELEN];
    unsigned int taille;
    unsigned short auteur;
    unsigned short index;
    struct objet *next;
};

extern struct objet *obj  ;
extern char volume[BLOCSIZE * BLOCNUM];
extern unsigned short FAT[BLOCNUM];
extern unsigned short freeblocks;


/**
\brief Cette fonction permet :
D'initialiser le tableau FAT en déclarant tous les blocs libres.
D'initialiser la variable freeblocks à BLOCNUM.
D'initialiser la variable obj
*/
void initialise_fat();
/*
{
    Pour i allant de 0 à BLOCNUM-1
        FAT[i] <- 0xFFFF
    Fin pour

    freeblocks <- BLOCNUM

    obj <- NULL
}
*/

/**
\brief Cette fonction permet de rechercher un objet par son nom dans la liste chaînée décrivant les objets
\param nom nom de l'objet à rechercher 
\return pointeur vers l'objet trouvé ou NULL sinon.
*/
struct objet *rechercher_objet(char *nom);
/*
{
    objet *o <- obj

    Tant que o.next != NULL
        Si o.nom == nom
            Retourner o
        Fin si

        o <- o.next
    Fin tant que

    Retourner NULL
}
*/

/** 
\brief Cette fonction permet de créer un objet en vérifiant qu'aucun objet n'a le même nom dans la liste (pas triée par nom)
si possible, de réserver des blocs dans le tableau FAT et de copier les données (data) dans les blocs pointés.
mettre à jour la variable freeblocks 
\param nom nom de l'objet 
\param auteur proprietaire de l'objet 
\param taille la taille de l'objet 
\param data les données à copier
*/
struct objet *creer_objet(char *nom, unsigned short auteur,unsigned int taille, char *data);
/*
{
    Si rechercher_objet(nom) == NULL
        Si freeblocks >= taille
            o.nom <- nom
            o.auteur <- auteur
            o.taille <- taille
            o.index <- 0
            o.next <- NULL

            Si obj.index == 0
                obj <- o
            Sinon
                objet *o2 <- obj
                Tant que o2.next != NULL
                    o2 <- o2.next
                Fin tant que
                o2.next <- o
            Fin si

            Pour i allant de 0 à taille-1
                FAT[i] <- 0
            Fin pour

            freeblocks <- freeblocks - taille

            Pour i allant de 0 à taille-1
                volume[FAT[i]] <- data[i]
            Fin pour

            Retourner o
        Sinon
            Retourner NULL
        Fin si
    Sinon
        Retourner NULL
    Fin si
}
*/

/**
\brief  Cette fonction permet de supprimer un objet trouvé par son nom, de libérer les blocs dans le tableau FAT, et de mettre à jour la variable freeblocks 
\param nom 
\return -1 si erreur, 0 sinon.
*/
int supprimer_objet(char *nom);
/*
{
    Si rechercher_objet(nom) != NULL
        objet *o <- obj
        objet *o2 <- obj

        Tant que o.next != NULL
            Si o.nom == nom
                o2.next <- o.next
                o.next <- NULL
                o <- o2
            Fin si

            o2 <- o
            o <- o.next
        Fin tant que

        Pour i allant de 0 à o.taille-1
            FAT[i] <- 1
        Fin pour

        freeblocks <- freeblocks + o.taille

        Retourner 0
    Sinon
        Retourner -1
    Fin si
}
*/


/** 
\brief Cette fonction permet :
De supprimer l'ensemble des objets
De liberer l'ensemble des blocs dans le tableau FAT
De modifier la variable freeblocks
*/
void supprimer_tout();
/*
{
    objet *o <- obj
    objet *o2 <- obj

    Tant que o.next != NULL
        o2 <- o
        o <- o.next
        o2.next <- NULL
    Fin tant que

    Pour i allant de 0 à BLOCNUM-1
        FAT[i] <- 1
    Fin pour

    freeblocks <- BLOCNUM
}
*/

/** POUR LES PLUS RAPIDES ..................** BONUS ** BONUS ** BONUS ** 
\brief Cette fonction permet :
De lire le contenu d'un objet et de le copier dans une structure de données allouée dynamiquement 
Attention à la taille !!!!!!!!!!!!!!
\param nom nom de l'objet
\return -1 si erreur, 0 sinon.
*/

int lire_objet(struct objet *o,char **data);
/*
{
    Si rechercher_objet(nom) != NULL
        objet *o <- rechercher_objet(nom)
        data <- malloc(o.taille)

        Pour i allant de 0 à o.taille-1
            data[i] <- volume[FAT[i]]
        Fin pour
        
        Retourner 0
    Sinon
        Retourner -1
    Fin si
}
*/

