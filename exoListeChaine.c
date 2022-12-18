#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Ici, on est obligé d'utiliser la notation struct xxx,
car la structure s'auto-référence!*/
typedef struct node {
	char data ;
	struct node *link ;
} Lnode ;

/* Insertion en "tête de liste" */
void insertionTete(Lnode **ph,char item){
	Lnode *newNode = malloc(sizeof(Lnode));

	newNode->data = item;
	newNode->link = *ph;

	*ph = newNode;
}

/* Insertion en "queue de liste" */
void insertionQueue(Lnode **ph,char item)	{
	Lnode *newNode = malloc(sizeof(Lnode));

	newNode->data = item;
	newNode->link = NULL;

	if(*ph == NULL) {
		*ph = newNode;
		return;
	}
	else {
		Lnode *current = *ph;
		while(current->link != NULL) {
			current = current->link;
		}
		current->link = newNode;
	}
}

/* Suppression en "tête de liste" */
void suppressionTete(Lnode **ph){
	if(*ph == NULL) {
		return;
	}

	Lnode *current = *ph;

	*ph = current->link;
	
	free(current);
}

/* Suppression en "Queue" de liste" */
void suppressionQueue(Lnode **ph){
	if (*ph == NULL || (*ph)->link == NULL) {
        *ph = NULL;
        return;
    }

	Lnode *current = *ph;
    while (current->link->link != NULL) {
        current = current->link;
    }
	
    free(current->link);
    current->link = NULL;
}

/* Procédure d'affichage de la liste. Ne doit pas être modifiée!!! */
void listeAffiche(Lnode * ptr){
	if ( NULL == ptr )
		printf("Liste vide!") ;
	else 
		printf("Contenu de la liste : ") ;
	while ( NULL != ptr ) 	{
		printf("%c ",ptr->data);
		ptr = ptr->link ;
		}
	printf("\n") ;
}

/* Programme principal. Ne doit pas être modifié!!! */
int main(void) {
	Lnode *tete = NULL ;

	listeAffiche(tete) ;
	insertionTete(&tete,'a') ;
	listeAffiche(tete) ;
	insertionTete(&tete,'c') ;
	listeAffiche(tete) ;
	insertionQueue(&tete,'t') ;
	listeAffiche(tete) ;
	insertionQueue(&tete,'s') ;
	listeAffiche(tete) ;
	suppressionTete(&tete) ;
	listeAffiche(tete) ;
	suppressionTete(&tete) ;
	listeAffiche(tete) ;
	suppressionQueue(&tete) ;
	listeAffiche(tete) ;
	suppressionTete(&tete) ;
	listeAffiche(tete) ;

   return EXIT_SUCCESS;
}	
