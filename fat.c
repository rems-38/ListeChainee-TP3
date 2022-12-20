#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fat.h"

struct objet *obj;
char volume[BLOCSIZE*BLOCNUM];
unsigned short FAT[BLOCNUM];
unsigned short freeblocks;

void initialise_fat() {
    for(int i = 0; i < BLOCNUM - 1; i++) {
        FAT[i] = 65535; // 0xFFFF -> 65535 (=> "libre")
    }

    freeblocks = BLOCNUM;

    obj = NULL;
}

struct objet *rechercher_objet(char *nom) {
    struct objet *current = obj;

    if (current != NULL) {
        while (current->next != NULL) {
            if (strcmp(current->nom, nom) == 0) {
                return current;
            }
            current = current->next;
        }
    }
    
    return NULL;
}

int rechercher_libre(unsigned short fat[BLOCNUM], int exclude) {
    for (int i = 0; i < BLOCNUM; i++) {
        if (i != exclude && fat[i] == 65535) {
            return i;
        }
    }
    return -1;
}

void print_fat() {
    for(int i = 0; i < 50; i++) {
        printf("i:%d | fat:%d \n", i, FAT[i]);
    }
}

void print_object(struct objet *obj) {
    if(obj != NULL) {
        struct objet *current = obj;
        while(current->next != NULL) {
            printf("-------------\n");
            printf("Name : %s\n", obj->nom);
            printf("Size : %d\n", obj->taille);
            printf("Autor : %d\n", obj->auteur);
            printf("Index : %d\n", obj->index);
            printf("--------------\n");
            current = current->next;
        }
    }   
}

struct objet *creer_objet(char *nom, unsigned short auteur, unsigned int taille, char *data) {
    if (rechercher_objet(nom) == NULL) {
        struct objet *new_objet = malloc(sizeof(struct objet));
        if (freeblocks >= taille/512 && rechercher_libre(FAT, -1) != -1) {
            strcpy(new_objet->nom, nom);
            new_objet->auteur = auteur;
            new_objet->taille = taille;
            new_objet->next = NULL;
            
            new_objet->index = rechercher_libre(FAT, -1); // -1 : ça n'exclu rien
            int index;
            for (int i = 0; i < (taille/512)+1; i++) {
                index = rechercher_libre(FAT, -1);
                FAT[index] = rechercher_libre(FAT, index);

                for (int j = i*512; j < (i+1)*512; j++) {
                    if (j < taille) {
                        volume[FAT[index]] = data[j];
                    }
                }
            }
            FAT[rechercher_libre(FAT, index)] = 65534;


            struct objet *current = obj;
            if (current == NULL) {
                obj = new_objet;
            }
            else {
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_objet;
            }

            freeblocks -= taille/512;

            return new_objet;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

int supprimer_objet(char *nom) {
    struct objet *to_delete = rechercher_objet(nom);
    if(to_delete == NULL) return -1;
    int index = to_delete->index;
    while (FAT[index] != 65534) { // 0xFFFE
        int temp_i = FAT[index];
        FAT[index] = 65535;
        index = temp_i;
    }
    FAT[index] = 65535;
    freeblocks += to_delete->taille;
    return 0;
}

int main() {
    initialise_fat();
    
    printf("Add 1er et 2ème objet\n");
    creer_objet("1er objet", 5, 513, "abcdefgh");
    creer_objet("2ème objet", 4, 1265, "fghdsjkc,vn buqcfbzqjq");
    print_fat();
    printf("\nDelete 1er objet\n");
    supprimer_objet("1er objet");
    print_fat();
    printf("\nAdd 3ème objet\n");
    creer_objet("3ème objet", 13, 1753, "ireznq");
    print_fat();


    
    return 0;
}
