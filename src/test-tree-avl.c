#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "tree-avl.h"

void monPrintF (void * a, void * b){
    printf("Valeur du noeud : %d\n", *(int*)a);
}

int compare(const void *data1, const void *data2) {

    int value1 = *((int *)data1);
    int value2 = *((int *)data2);

    if (value1 < value2) {
        return -1;
    } else if (value1 > value2) {
        return 1;
    } else {
        return 0;
    }
}

void delete(void *data){
    free(data);
}

/**
 * Test réalisés pour les arbres binaires
 * Affichage des résultats des tris
 */
void testArbresAVL(void) {
    int i = 5, j = 3, k = 8, m = 1, l = 4, n = 9;
    size_t sizeInt = sizeof(int);

    Tree racine = tree_create(&i, sizeInt);
    Tree fils1 = tree_create(&j, sizeInt);
    Tree fils2 = tree_create(&k, sizeInt);

    tree_set_left(racine, fils1);
    tree_set_right(racine, fils2);

    Tree fils1fils1 = tree_create(&m, sizeInt);
    Tree fils2fils1 = tree_create(&l, sizeInt);

    tree_set_left(fils1, fils1fils1);
    tree_set_right(fils1, fils2fils1);

    printf("\nTri in-order : \n");
    tree_in_order(racine, monPrintF, NULL);

    printf("\nAjout d'un nouveau fils\n");
    Tree fils2fils2 = tree_insert(&fils2, &n, sizeInt, compare)->right;
//    printf("\nFils 2 du fils 2 : %d (Parent : %d)\n", *fils2fils2->data, *fils2fils2->parent->data);

    printf("\nNouveau tri in-order : \n");
    tree_in_order(racine, monPrintF, NULL);

    printf("\nAffichage du nouveau fils par recherche : \n");
    tree_search(racine, &l, compare);

    printf("\nSuppression d'un fils\n");
    _tree_remove(&fils2, &k, sizeInt, compare, NULL);

    printf("\nNouveau tri in-order : \n");
    tree_in_order(racine, monPrintF, NULL);
}

int main() {
    testArbresAVL();

    return EXIT_SUCCESS;
}