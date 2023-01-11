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

void print_fat(int nb_blocs) {
    for(int i = 0; i < nb_blocs; i++) {
        i < 10 ? printf("i:%d  | fat:%d \n", i, FAT[i]) : printf("i:%d | fat: %d \n", i, FAT[i]);
    }
}

void print_volume(int nb_blocs) {
    for(int i = 0; i < nb_blocs; i++) {
        printf("Bloc %d : \n", i);
        for(int j = 0; j < BLOCSIZE; j++) {
            printf("%c", volume[i*BLOCSIZE+j]);
        }
        printf("\n");
    }
}


void print_objects() {
    if(obj != NULL) {
        struct objet *current = obj;
        int i = 0;
        while(current != NULL) {
            printf("--------------\n");
            printf("ID : %d\n", i);
            printf("Name : %s\n", current->nom);
            printf("Size : %d\n", current->taille);
            printf("Autor : %d\n", current->auteur);
            printf("Index : %d\n", current->index);
            printf("--------------\n");
            i++;
            current = current->next;
        }
    }   
}

struct objet *creer_objet(char *nom, unsigned short auteur, unsigned int taille, char *data) {
    if (rechercher_objet(nom) == NULL) {
        struct objet *new_objet = malloc(sizeof(struct objet));
        if (freeblocks >= taille/BLOCSIZE && rechercher_libre(FAT, -1) != -1) {
            strcpy(new_objet->nom, nom);
            new_objet->auteur = auteur;
            new_objet->taille = taille;
            new_objet->next = NULL;
            
            new_objet->index = rechercher_libre(FAT, -1); // -1 : ça n'exclu rien
            int index;
            int act_size = 0;
            for (int i = 0; i < (taille/BLOCSIZE)+1; i++) {
                index = rechercher_libre(FAT, -1);
                FAT[index] = rechercher_libre(FAT, index);

                for (int j = (FAT[index]-1)*BLOCSIZE; act_size < taille && j < (FAT[index])*BLOCSIZE; j++, act_size++) {
                    volume[j] = data[act_size];
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

            freeblocks -= taille/BLOCSIZE;

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

    if(obj == to_delete) {
        obj = obj->next;
    }
    else {
        while(obj->next != to_delete) {
            obj = obj->next;
        }
        obj->next = obj->next->next;
    }

    freeblocks += to_delete->taille/BLOCSIZE;
    free(to_delete);
    return 0;
}

void supprimer_tout() {
    while(obj != NULL) {
        supprimer_objet(obj->nom);
        obj = obj->next;
    }
}

int main() {
    initialise_fat();
    
    creer_objet("1er objet", 5, 680, "abcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefghabcdefgh");
    // print_volume(3);
    creer_objet("2ème objet", 4, 9, "fgadsjkcq");
    // print_volume(3);
    creer_objet("3ème objet", 13, 6, "ireznq");
    print_volume(7);
    print_fat(10);

    
    return 0;
}
