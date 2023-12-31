#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct _TreeNode *Tree;

typedef enum{
    Red = 0,
    Black = 1
} Color;

struct _TreeNode {
    Tree left;
    Tree right;
    Tree parent;
    Color color;
    char data[1];
};

Tree tree_new();

void tree_delete(Tree tree, void (*delete) (void *));

Tree tree_create(const void *data, size_t size);

Tree tree_get_left(Tree tree);

Tree tree_get_right(Tree tree);

void *tree_get_data(Tree tree);

bool tree_set_left(Tree tree, Tree left);

bool tree_set_right(Tree tree, Tree right);

bool tree_set_data(Tree tree, const void *data, size_t
size);

void tree_pre_order(Tree tree,
                    void (*func)(void *, void *),
                    void *extra_data); // VGD

void tree_in_order(Tree tree,
                   void (*func)(void *, void *),
                   void *extra_data); // GVD

void tree_post_order(Tree tree,
                     void (*func)(void *, void *),
                     void *extra_data); // GDV

size_t tree_height(Tree tree);

size_t tree_size(Tree tree);

bool tree_insert_sorted(Tree *ptree,
                        const void *data,
                        size_t size,
                        int (*compare)(const void *, const void *));

void *tree_search(Tree tree,
                  const void *data,
                  int (*compare)(const void *, const
                  void *));

int tree_sort(void *array,
              size_t length,
              size_t size,
              int (*compare)(const void *, const void
              *));

// TESTS
void rotate_left(Tree *tree);
void rotate_right(Tree *tree);

void tree_get_parent(Tree tree);
Tree tree_get_uncle (Tree tree);
Tree tree_get_grandparent (Tree tree);
Tree tree_get_root (Tree tree);


void insert_case1 (Tree ptree);
void insert_case2 (Tree ptree);
void insert_case3 (Tree ptree);
void insert_case4 (Tree ptree);
void insert_case5 (Tree ptree);

Tree tree_find_min(Tree tree);
Tree tree_find_max(Tree tree);

void * delete(Tree tree);


#endif
