#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_etu.h"
#include "list.h"
#include "fonc_etu.h"
#include "test_fonc.h"

int main(){
    /*
    printf("\nTest de tree_resetByteOcc\n");
    test_resetByteOcc();
    printf("\nTest de tree_resetByteCod\n");
    test_resetByteCod();
    printf("\n");
    */
    struct byteCoding T[256];
    char buffer[1024];
    //strcpy(buffer, "aaaaa");
    strcpy(buffer, "test");
    //strcpy(buffer, "tester");
    printf("\nchaine de test : %s\n", buffer);
    tree_countByteOccurrence(buffer, strlen(buffer), T);
    printf("\nAffichage des valeurs des byteCode\n");
    tree_displayByteCoding(T);
    struct tree_node * arbre = tree_create(T);
    printf("\ncode de Huffman\n");
    tree_buildHuffmanCode(arbre, 0, 0);
    printf("\nAffichage de l'arbre\n");
    tree_display(arbre, 0);
    printf("\nVerification des prefixes\n");
    struct tree_node * fils = arbre->fils_gauche;
    int code = *(fils->valeur);
    if( verif_prefixe(arbre, conversion_binaire(code), 0) == 1 ){
        printf("Erreur, un code est prefixe d'un autre.\n");
    } else {
        printf("Aucun code n'est prefixe d'un autre.\n");
    }
    printf("\nlibération de la mémoire :\n");
    tree_destroy(arbre);
}