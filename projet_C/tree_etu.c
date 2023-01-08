#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_etu.h"
#include "list.h"
#include "fonc_etu.h"
#include "test_fonc.h"

// gestion du tableau detest_createNode(); codage des octets

    // remet a zero le nombre d'occurrences des octet
void tree_resetByteOccurrence( struct byteCoding indexedCodeTable[256] ){
    int i = 0;
    for( i = 0; i < 256; i++ ){
        indexedCodeTable[i].occurrence = 0;
        indexedCodeTable[i].byte = i;
    }
}

   	// remet a zero les codes oct
void tree_resetByteCoding( struct byteCoding indexedCodeTable[256] ){
    int i = 0;
    for( i = 0; i < 256; i++ ){
        indexedCodeTable[i].huffmanCode = 0;
        indexedCodeTable[i].nbBits = 0;
        indexedCodeTable[i].byte = i;
    }
}

    // compte de nombre d'occurrence des octets dans un tampon
void tree_countByteOccurrence( const char * buffer, int size, struct byteCoding indexedCodeTable[256] ){
    int i;
    tree_resetByteOccurrence(indexedCodeTable);
    unsigned int k;
    for( i = 0; i < size; i++){
        k = *(buffer + i);
        indexedCodeTable[k].occurrence += 1;
    }
}

/* gestion de l'arbre de Huffmanconst t_byte * buffer */

    // affiche la table de codage des valeurs d'octet pour debug
void tree_displayByteCoding(struct byteCoding indexedCodeTable[256]){
    int i;
    for( i = 0; i < 256; i++){
        if( indexedCodeTable[i].occurrence != 0 ){
            printf("%d ", indexedCodeTable[i].occurrence);
            printf("%c ", indexedCodeTable[i].byte);
        }
    }
    printf("\n");
}

	// creation d'un noeudint j;
struct tree_node * tree_createNode (struct tree_node * father, int * value ){
    struct tree_node * nouveau_noeud = (struct tree_node *)malloc(sizeof(struct tree_node));
    if( nouveau_noeud == NULL ) return NULL;
    nouveau_noeud->valeur = value;
    printf("valeur = %d\n", *value);
    nouveau_noeud->pere = father;
    nouveau_noeud->fils_droit = NULL;
    nouveau_noeud->fils_gauche = NULL;
    return nouveau_noeud;
}

   	// creation d'un noeud de codage et liaison avec ses fils
struct tree_node * tree_createCodingNode ( struct tree_node * left, struct tree_node * right ){
    struct tree_node * nouveau_noeud = (struct tree_node *)malloc(sizeof(struct tree_node));
    if( nouveau_noeud == NULL ) return NULL;
    nouveau_noeud->pere = NULL;
    nouveau_noeud->fils_droit = right;
    nouveau_noeud->fils_gauche = left;
    int * p = (int*)malloc(sizeof(int));
    if( p == NULL ){
        free(nouveau_noeud);
        return NULL;
    }
    if( right != NULL ){
        *p = *(right->valeur) + *(left->valeur);
        nouveau_noeud->valeur = p;
        right->pere = nouveau_noeud;
        left->pere = nouveau_noeud;
        printf("valeur codingNode = %d\n", *(nouveau_noeud->valeur));
    } else {
        *p = *(left->valeur);
        nouveau_noeud->valeur = p;
        left->pere = nouveau_noeud;
        printf("valeur codingNode = %d\n", *(nouveau_noeud->valeur));
    }
    return nouveau_noeud;
}

	// creation de l'arbre
struct tree_node * tree_create ( struct byteCoding * indexedCodeTable ){
    printf("\nCreation de l'arbre\n");
    // création de la liste
    s_node * liste = list_create(), * ptr;
    int i;  
    for( i = 0; i < 256; i++){
        if( indexedCodeTable[i].occurrence > 0 ){
            struct tree_node * nouveau_noeud = tree_createNode(NULL, &(indexedCodeTable[i].occurrence));
            list_orderedAppend(&liste, &CompNode, nouveau_noeud);
        }
    }
    // création de l'arbre
    ptr = liste;
    struct tree_node * codNode;
    // cas où un seul noeud
    if( list_next(ptr) == NULL ){
        struct tree_node * noeud_solitaire = (struct tree_node *)GET_LIST_DATA(ptr, struct tree_node *);
        ptr = list_headRemove(ptr);
        struct tree_node * nouveau_codNode = tree_createCodingNode(noeud_solitaire, NULL);
        list_orderedAppend(&ptr, &CompNode, nouveau_codNode);
    } else {
        // cas où n noeud
        while( list_next(ptr) != NULL ){
            struct tree_node * NoeudInter = (struct tree_node *)GET_LIST_DATA(ptr, struct tree_node *);
            ptr = list_headRemove(ptr);
            struct tree_node * NoeudInter2 = (struct tree_node *)GET_LIST_DATA(ptr, struct tree_node *);
            ptr = list_headRemove(ptr);
            struct tree_node * nouveau_codNode = tree_createCodingNode(NoeudInter, NoeudInter2);
            list_orderedAppend(&ptr, &CompNode, nouveau_codNode);
        }
    }
    codNode = (struct tree_node *)GET_LIST_DATA(ptr, struct tree_node *);
    return codNode;
}

    // destruction d'un arbre et liberation des donnees 
	// (sauf au niveau des feuilles)
void tree_destroy( struct tree_node * root ){
    struct tree_node * noeud = root;
    if( noeud->fils_gauche != NULL ){
        tree_destroy(noeud->fils_gauche);
    }
    if( noeud->fils_droit != NULL ){
        tree_destroy(noeud->fils_droit);
    }
    printf("libération de %d\n", *(noeud->valeur));
    if( !((noeud->fils_gauche == NULL) && (noeud->fils_droit == NULL)) ){
        free(noeud->valeur);
    }
    free(noeud);
}

    // construit les codes de huffman en parcourant l'arbre
void tree_buildHuffmanCode(struct tree_node * root, int level, int code){
    struct tree_node * noeud = root;
    if( ( noeud->fils_gauche != NULL) ){
        tree_buildHuffmanCode(noeud->fils_gauche, level + 1, code << 1);
    }
    if( ( noeud->fils_droit != NULL ) ){
        tree_buildHuffmanCode(noeud->fils_droit, level + 1, (code << 1) + 1 );
    }
    if( (noeud->fils_gauche == NULL) && (noeud->fils_droit == NULL) ){
        struct byteCoding * bCode = (struct byteCoding *) noeud->valeur;
        bCode->huffmanCode = code;
        if( code < 2 ){
            bCode->nbBits = 1;
        } else {
            bCode->nbBits = level;
        }
        printf("niveau %d -- noeud %d -- Huffman %d -- binaire %d\n", level, *(noeud->valeur), bCode->huffmanCode, conversion_binaire(bCode->huffmanCode));
        if( code == 0 ){
            code++;
        }
    }
}

    // affichage de l'arbre
void tree_display( struct tree_node * root, int level ){
    struct tree_node * noeud = root;
    if( ( noeud->fils_gauche != NULL) ){
        tree_display(noeud->fils_gauche, level + 1);
    }
    if( ( noeud->fils_droit != NULL ) ){
        tree_display(noeud->fils_droit, level + 1);
    }
    if( (noeud->fils_gauche == NULL) && (noeud->fils_droit == NULL) ){
        struct byteCoding * bCode = (struct byteCoding *) noeud->valeur;
        printf("niveau %d -- noeud %d -- nbBits %d -- code de Huffman %d -- FEUILLE\n", level, *(noeud->valeur), bCode->nbBits, conversion_binaire(bCode->huffmanCode));
    } else {
        printf("niveau %d -- noeud %d -- NOEUD\n", level, *(noeud->valeur));
    }
} 