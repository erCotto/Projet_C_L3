#include "tree_etu.h"
#include "test_fonc.h"
#include "list.h"
#include <stdio.h>

// fonction de test de tree_resetByteOccurrence
void test_resetByteOcc(){
    int i;
    struct byteCoding T[256];
    for( i = 0; i < 256; i++ ){
        T[i].occurrence = i + 1;
    }
    tree_displayByteCoding(T);
    printf("\nAppel de tree_resetByteOccurrence\n");
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
        printf("%d ", T[i].huffmanCode);
    }
    printf("\n\nAppel de tree_resetByteCoding\n");
    tree_resetByteCoding(T);
    for( i = 0; i < 256; i++){
        printf("%d ", T[i].huffmanCode);
    }
}

// fonction de comparaison
int CompNode(s_node * s, void * param){
    struct tree_node * noeud = (struct tree_node *)param;
    int * paramInt = noeud->valeur;
    //printf("paramInt = %d\n", *paramInt);
    struct tree_node * NoeudInter = (struct tree_node *)GET_LIST_DATA(s, struct tree_node *);
    int * paramInt2 = NoeudInter->valeur;
    printf("data_node = %d\n", *paramInt2);
    if( *paramInt2 == *paramInt) return 0;
    if( *paramInt2 < *paramInt) return -1;
    else return 1;
}

// fonction de conversion en binaire
int conversion_binaire(int param) {
    if( param == 0 ) return 0;
    if( param == 1 ) return 1;
    int res = ( param % 2 ) + 10 * conversion_binaire( param / 2 );
    return res;
}

// fonction de vérification des préfixes
int verif_prefixe(struct tree_node * root, int prefixe, int vrai){
    if( vrai == 0 ){
        struct tree_node * noeud = root;
        if( noeud->fils_gauche != NULL ){
            struct byteCoding * bCode = (struct byteCoding *) noeud->valeur;
            if( (conversion_binaire(bCode->huffmanCode) == prefixe) || (conversion_binaire(bCode->huffmanCode) == prefixe * 10) ){
                vrai = 1;
            }
            vrai = verif_prefixe(noeud->fils_gauche, conversion_binaire(bCode->huffmanCode), vrai);
        }
        if( noeud->fils_droit != NULL ){
            struct byteCoding * bCode = (struct byteCoding *) noeud->valeur;
            if( (conversion_binaire(bCode->huffmanCode) == prefixe) || (conversion_binaire(bCode->huffmanCode) == prefixe * 10) ){
                vrai = 1;
            }
            vrai = verif_prefixe(noeud->fils_droit, conversion_binaire(bCode->huffmanCode), vrai);
        }
    }
    return vrai;
}