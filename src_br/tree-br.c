#include <string.h>
#include "tree-br.h"
#include <stdbool.h>
#include "min-max.h"
#include <stdlib.h>

/*--------------------------------------------------------------------*/
Tree tree_new() {
    return NULL;
}

void tree_delete(Tree tree, void (*delete)(void *)) {
    if (tree) {
        tree_delete(tree->left, delete);
        tree_delete(tree->right, delete);
        if (delete)
            delete(tree->data);
        free(tree);
    }
}

Tree tree_create(const void *data, size_t size) {
    Tree tree = (Tree) malloc(3 * sizeof(Tree) + size + sizeof(size_t));

    if (tree) {
        tree->left = NULL;
        tree->right = NULL;
        tree->parent = NULL;
        tree->color = Red;
        memcpy(tree->data, data, size);
    }
    return tree;
}

Tree tree_get_left(Tree tree) {
    if (tree)
        return tree->left;
    else
        return NULL;
}

Tree tree_get_right(Tree tree) {
    if (tree)
        return tree->right;
    else
        return NULL;
}

void *tree_get_data(Tree tree) {
    if (tree)
        return tree->data;
    else
        return NULL;
}

bool tree_set_left(Tree tree, Tree left) {
    if (tree) {
        tree->left = left;
        return true;
    } else
        return false;
}

bool tree_set_right(Tree tree, Tree right) {
    if (tree) {
        tree->right = right;
        return true;
    } else
        return false;
}

bool tree_set_data(Tree tree, const void *data, size_t
size) {
    if (tree) {
        memcpy(tree->data, data, size);
        return true;
    } else
        return false;
}

void tree_pre_order(Tree tree,
               void (*func)(void *, void *),
               void *extra_data) {
    if (tree) {
        func(tree->data, extra_data);
        tree_pre_order(tree->left, func, extra_data);
        tree_pre_order(tree->right, func, extra_data);
    }
}

void tree_in_order(Tree tree,
              void (*func)(void *, void *),
              void *extra_data) {
    if (tree) {
        tree_in_order(tree->left, func, extra_data);
        func(tree->data, extra_data);
        tree_in_order(tree->right, func, extra_data);
    }
}

void tree_post_order(Tree tree,
                void (*func)(void *, void *),
                void *extra_data) {
    if (tree) {
        tree_post_order(tree->left, func, extra_data);
        tree_post_order(tree->right, func, extra_data);
        func(tree->data, extra_data);
    }
}

size_t tree_height(Tree tree) {
    if (tree)
        return 1 + MAX (tree_height(tree->left),
                        tree_height(tree->right));
    else
        return 0;
}

size_t tree_size(Tree tree) {
    if (tree)
        return 1 + tree_size(tree->left) + tree_size
                (tree->right);
    else
        return 0;
}

bool tree_insert_sorted(Tree *ptree,
                   const void *data,
                   size_t size,
                   int (*compare)(const void *, const
                   void *)) {
    if (*ptree) {
        switch (compare(data, (*ptree)->data)) {
            case -1:
                return tree_insert_sorted(&(*ptree)->left,
                                          data, size, compare);
            default:
                return tree_insert_sorted(&(*ptree)->right,
                                          data, size, compare);
        }
    } else {
        Tree new = tree_create(data, size);
        if (new) {
            *ptree = new;
            return true;
        } else
            return false;
    }

}

void *tree_search(Tree tree,
            const void *data,
            int (*compare)(const void *, const void
            *)) {
    if (tree) {
        switch (compare(data, tree->data)) {
            case -1:
                return tree_search(tree->left, data, compare);
            case 0:
                return tree->data;
            case 1:
                return tree_search(tree->right, data, compare);
            default:
                return NULL; // RAJOUTE PARCE QUE WARNING !!!
        }
    } else
        return NULL;
}

static void set(void *data, void *array) {
    static size_t size;
    static size_t offset;

    if (data) {
        memcpy(array + offset, data, size);
        offset += size;
    } else {
        offset = 0;
        size = *(size_t *) array;
    }
}

int tree_sort(void *array,
          size_t length,
          size_t size,
          int (*compare)(const void *, const void *)) {
    size_t i;
    Tree tree = tree_new();
    void *pointer;

    pointer = array;
    for (i = 0; i < length; i++) {
        if (tree_insert_sorted(&tree, pointer, size, compare))
            pointer += size;
        else {
            tree_delete(tree, NULL);
            return false;
        }
    }

    set(NULL, &size);
    tree_in_order(tree, set, array);
    tree_delete(tree, NULL);

    return true;
}

//Fonctions ajoutées pour le TEA.


Tree rotate_left(Tree node) {
    if (!node) {
        return node;
    }

    // Rotate right on the right subtree.
    if (node->right) {
        node->right = rotate_right(&node->right);
    }

    // Rotate left on the original node.
    return rotate_left(&node);
}
Tree rotate_right(Tree node) {
    if (!node) {
        return node;
    }

    // Rotate left on the left subtree.
    if (node->left) {
        node->left = rotate_left(&node->left);
    }

    // Rotate right on the original node.
    return rotate_right(node);
}

Tree tree_get_parent(Tree tree){
    if (tree)
        return tree->parent;
    else
        return NULL;
}

Tree tree_get_grandparent(Tree tree){
    //If the tree exists and the current node has a parent, then return the grandparent (parent of the parent).
    if ((tree != NULL) && (tree->parent != NULL))
        return tree->parent->parent;
    else
        return NULL;
}

Tree tree_get_uncle(Tree tree){
    Tree grandparent = tree_get_grandparent(tree);
    //If there is no parent of parent, return NULL
    if (grandparent == NULL)
        return NULL;
    //If the grandparent has an uncle (different node on the same "row" of the parent), return it if it's on the left or the right.
    if (tree->parent == grandparent->left){
        return grandparent->right;
    }else{
        return grandparent->left;
    }
}

Tree tree_get_father(Tree tree){
    if (tree)
        //If the tree exists, return its parent
        return tree->parent;
    else
        return NULL;
}

bool tree_set_father (Tree tree, Tree parent){
    if (tree)
    {
        tree->parent = parent;
        return true;
    }
    else
        return false;
}

Tree tree_get_root(Tree tree){
    //If the father (parent) of the node is null, then return the root of the tree, if it's not, it is done again until reaching the top.
    if(tree_get_father(tree) == NULL){
        return tree;
    }else{
        return tree_get_root(tree_get_father(tree));
    }
}

//Insert and it's many cases
void insert(Tree tree, const void * data){

    if ((*(int*)data) < *(int*)tree_get_data(tree)){
        /*The data will be inserted into the tree according to five cases,
         each being a condition of insertion depending on the state of the tree.*/
        if (tree_get_left(tree) == NULL){
            Tree leftTree = tree_create(data, sizeof(data));

            tree_set_left(tree, leftTree);
            tree_set_parent(leftTree, tree);

            insert_case1(leftTree);
        }else{
            insert(tree_get_left(tree), data);
        }
    }else{
        if (tree_get_right(tree) == NULL){
            Tree rightTree = tree_create(data, sizeof(data));

            tree_set_right(tree, rightTree);
            tree_set_parent(rightTree, tree);

            insert_case1(rightTree);
        }else{
            insert(tree_get_right(tree), data);
        }
    }
}

void insert_case1(Tree ptree){
    //If the tree has no parent, then it is the root and must be colored Black.
    if(ptree->parent == NULL)
        ptree->color = Black;
    else
        insert_case2(ptree);
}

void insert_case2(Tree ptree){
    //If the parent's color is black, then the child is colored Black as well.
    if((ptree)->parent->color == Black)
        return;
    else
        insert_case3(ptree);
}

void insert_case3(Tree ptree){
    Tree uncle = tree_get_uncle(ptree);
    Tree grandparent;
    //If the uncle isn't NULL and the uncle is colored Red, then the parent of ptree is colored Black and its grandparent is colored Red.
    if((uncle != NULL) && (uncle->color == Red)){
        ptree->parent->color = Black;
        uncle->color = Black;
        grandparent = tree_get_grandparent(ptree);
        grandparent->color = Red;
        insert_case1(grandparent);
    }
    else
        insert_case4(ptree);
}

void insert_case4(Tree ptree){
    Tree grandparent =tree_get_grandparent(ptree);

    if((ptree == (ptree)->parent->right) && ((ptree)->parent == grandparent->left)){
        rotate_left((ptree)->parent);
        ptree = (ptree)->left;
    }
    else if((ptree == (ptree)->parent->left) && ((ptree)->parent == grandparent->right)){
        rotate_right((ptree)->parent);
        ptree = (ptree)->right;
    }
    insert_case5(ptree);
}

void insert_case5(Tree ptree){
    Tree grandparent = tree_get_grandparent(ptree);

    (ptree)->parent->color = Black;
    grandparent->color = Red;
    if((ptree == (ptree)->parent->left) && ((ptree)->parent == grandparent->left)){
        rotate_right(grandparent);
    }
    else{
        rotate_left(grandparent);
    }

}

//Delete

Tree tree_find_min(Tree tree){
    if (tree == NULL)
        return NULL;
    if (tree_get_left(tree) == NULL)
        return tree;
    else
        return tree_find_min(tree_get_left(tree));
}

Tree tree_find_max(Tree tree){
    if (tree == NULL)
        return NULL;
    if (tree_get_right(tree) == NULL)
        return tree;
    else
        return tree_find_max(tree_get_right(tree));
}

void * delete(Tree tree){
    if (tree == NULL)
        return NULL;
    //Check if it's a node.
    if (tree_get_left(tree) == NULL && tree_get_right(tree) == NULL){
        //Check if it's not the root
        if (tree_get_father(tree) != NULL){
            if (tree_get_father(tree)->left == tree){
                tree_set_left(tree_get_father(tree), NULL);
            }
            else{
                tree_set_right(tree_get_father(tree), NULL);
            }
        }
        //Delete node
        tree_delete(tree, 0);
        return NULL;

        //Check if node has a right child
    }else if (tree_get_left(tree) == NULL){
        //Check if it's not root
        if (tree_get_father(tree) != NULL){
            if (tree_get_father(tree)->left == tree){
                tree_set_left(tree_get_father(tree), tree_get_right(tree));
            }else{
                tree_set_right(tree_get_father(tree), tree_get_right(tree));
            }
        }
        //Delete node
        tree_delete(tree, 0);
        return NULL;

    }else if (tree_get_right(tree) == NULL){
        if (tree_get_father(tree) != NULL){
            if (tree_get_father(tree)->left == tree){
                tree_set_left(tree_get_father(tree), tree_get_left(tree));
            }
            else{
                tree_set_right(tree_get_father(tree), tree_get_left(tree));
            }
        }

        tree_delete(tree, 0);
        return NULL;

    }else{
        Tree min = tree_find_min(tree_get_right(tree));
        void * data = tree_get_data(tree);

        tree_set_data(tree, tree_get_data(min), sizeof(int));
        tree_set_data(min, data, sizeof(int));

        return delete(min);
    }
}