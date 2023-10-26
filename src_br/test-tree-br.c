#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "tree-br.h"

void monPrintF (void * a, void * b){
    printf("Node value : %d\n", *(int*)a);
}

/**
 * Tests for the Red and Black tree.
 * Displaying creation, deletion and sorting of red and black tree.
 */
void testRedBlackTree(void){
    int i = 10, j = 85, k = 15, m = 70, n = 20, o = 60, p = 30, q = 50, r = 65, s = 80, t = 90, u = 40, v = 5, w = 55;
    size_t sizeInt = sizeof(int);

    printf("\n-----Creation of the tree-----\n\n");

    Tree root = tree_create(&i, sizeInt);
    insert_case1(root);
    insert(tree_get_root(root), &j);
    insert(tree_get_root(root), &k);
    insert(tree_get_root(root), &m);
    insert(tree_get_root(root), &n);
    insert(tree_get_root(root), &o);
    insert(tree_get_root(root), &p);
    insert(tree_get_root(root), &q);
    insert(tree_get_root(root), &r);
    insert(tree_get_root(root), &s);
    insert(tree_get_root(root), &t);
    insert(tree_get_root(root), &u);
    insert(tree_get_root(root), &v);
    insert(tree_get_root(root), &w);

    printf("Tri pre-order : \n");
    tree_pre_order(root, monPrintF, NULL);

    printf("\nTri post-order : \n");
    tree_post_order(root, monPrintF, NULL);

    printf("\nTri in-order : \n");
    tree_in_order(root, monPrintF, NULL);

    tree_delete(racine, 0);
}

int main(){
    testRedBlackTree();

    return EXIT_SUCCESS;
}