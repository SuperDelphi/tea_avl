#ifndef _TREE_H_
#define _TREE_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct _TreeNode *Tree;

struct _TreeNode {
    Tree left;
    Tree right;
    Tree parent;
    char data[1];
    size_t balance;
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

//bool tree_insert_sorted(Tree *ptree,
//                        const void *data,
//                        size_t size,
//                        int (*compare)(const void *, const void *));

void *tree_search(Tree tree,
                  const void *data,
                  int (*compare)(const void *, const
                  void *));

int tree_sort(void *array,
              size_t length,
              size_t size,
              int (*compare)(const void *, const void
              *));

// Ajout fonctions du TEA
int compare(const void *data1, const void *data2);

void rotation_left(Tree tree);

void rotation_right(Tree tree);

void balance(Tree *ptree);

Tree tree_insert(Tree *ptree,
                 const void *data,
                 size_t size,
                 int (*compare)(const void *, const void *));

Tree tree_min(Tree tree);

Tree _tree_remove(Tree *ptree,
                         const void *data,
                         size_t size,
                         int (*compare)(const void *, const void *),
                         void (*delete)(void *));
#endif
