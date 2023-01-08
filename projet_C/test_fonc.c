#include "tree_etu.h"
#include "test_fonc.h"
#include <stdio.h>

int main(){
    printf("%d\n", 1 >> 1);
}

// fonction de test de tree_resetByteOccurrence
void test_resetByteOcc(){
    int i;
    struct byteCoding T[256];
    for( i = 0; i < 256; i++ ){
        T[i].occurrence = i + 1;
    }
    tree_displayByteCoding(T);
    printf("\nAppel de tree_resetByteOccurrence\n\n");
    tree_resetByteOccurrence(T);
    tree_displayByteCoding(T);
}

// fonction de test de tree_resetByteCoding
void test_resetByteCod(){
    int i;
    struct byteCoding T[256];
    for( i = 0; i < 256; i++ ){
        T[i].huffmanCode = i + 1;
    }
    for( i = 0; i < 256; i++){
        printf("%d\n", T[i].huffmanCode);
    }
    tree_resetByteCoding(T);
    for( i = 0; i < 256; i++){
        printf("%d\n", T[i].huffmanCode);
    }
}

// fonction de vérification des préfixes
int verif_prefixe(struct tree_node * root, int prefixe, int vrai){
    if( vrai == 0 ){
        struct tree_node * noeud = root;
        if( noeud->fils_gauche != NULL ){
            struct byteCoding * bCode = (struct byteCoding *) noeud->valeur;
            if( (bCode->huffmanCode == prefixe) || (bCode->huffmanCode == prefixe * 10) ){
                vrai = 1;
            }
            verif_prefixe(noeud->fils_gauche, code, vrai);
        }
        if( noeud->fils_droit != NULL ){
            struct byteCoding * bCode = (struct byteCoding *) noeud->valeur;
            if( (bCode->huffmanCode == prefixe) || (bCode->huffmanCode == prefixe * 10) ){
                vrai = 1;
            }
            verif_prefixe(noeud->fils_droit, code, vrai);
        }
        return vrai;
    }
    return vrai;
}