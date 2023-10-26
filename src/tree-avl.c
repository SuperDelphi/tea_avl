#include <string.h>
#include "tree-avl.h"
#include <stdbool.h>
#include "min-max.h"

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
     memcpy(tree->data, data, size);
        tree->balance = 0;
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

// Fonctions ajoutées pour le TEA.

void rotation_left(Tree tree) {
    Tree newParent = tree->left;
    Tree children = newParent->right;

    newParent->right = tree;
    tree->left = children;

    tree->balance = tree_height(tree->left) - tree_height(tree->right);
    newParent->balance = tree_height(newParent->left) - tree_height(newParent->right);
}

void rotation_right(Tree tree) {
    Tree newParent = tree->right;
    Tree children = newParent->left;

    newParent->left = tree;
    tree->right = children;

    tree->balance = tree_height(tree->left) - tree_height(tree->right);
    newParent->balance = tree_height(newParent->left) - tree_height(newParent->right);
}

//void rotation_left_delphi(Tree *tree) {
//    if ((*tree)->right != NULL) {
//        (*tree)->left = (*tree);
//        (*tree)->left->parent = (*tree)->right;
//        (*tree)->right->parent = (*tree)->parent;
////        (*tree)->right->left = (*tree)->left;
//        (*tree) = (*tree)->right;
//    }
//}
//
//Tree rotate_right_left(Tree node) {
//    if (!node) {
//        return node;
//    }
//
//    // Rotation droite (droite-droite) sur le sous-arbre droit.
//    if (node->right) {
//        node->right = rotate_right(node->right);
//    }
//
//    // Rotation gauche (gauche-gauche) sur le nœud d'origine.
//    return rotate_left(node);
//}
//Tree rotate_left_right(Tree node) {
//    if (!node) {
//        return node;
//    }
//
//    // Rotation gauche (gauche-gauche) sur le sous-arbre gauche.
//    if (node->left) {
//        node->left = rotate_left(node->left);
//    }
//
//    // Rotation droite (droite-droite) sur le nœud d'origine.
//    return rotate_right(node);
//}
//
//
//void rotation_right_delphi(Tree *tree) {
////    if (tree->left != NULL) {
////        tree->right = tree;
////        tree = tree->left;
////    }
//}
static Tree _tree_remove(Tree *ptree, const void *data, size_t size, int (*compare)(const void *, const void *), void (*delete)(void *)) {
    Tree tree = *ptree;

    if (tree == NULL) {
        return NULL; // Le nœud à supprimer n'existe pas, retourne NULL.
    }

    int C = compare(data, tree->data);

    if (C < 0) {
        tree->left = _tree_remove(&tree->left, data, size, compare, delete);
    } else if (C > 0) {
        tree->right = _tree_remove(&tree->right, data, size, compare, delete);
    } else {
        // Le nœud à supprimer a été trouvé
        if (tree->left == NULL) {
            Tree temp = tree->right;
            if (delete) {
                delete(tree->data);
            }
            free(tree);
            return temp;
        } else if (tree->right == NULL) {
            Tree temp = tree->left;
            if (delete) {
                delete(tree->data);
            }
            free(tree);
            return temp;
        } else {
            // Cas où le nœud a deux sous-arbres
            // Trouver le nœud minimal du sous-arbre droit
            Tree min_right = tree_search(tree->right, data, compare);
            // Copier la valeur du nœud minimal dans le nœud à supprimer
            memcpy(tree->data, min_right->data, size);
            // Supprimer le nœud minimal du sous-arbre droit
            tree->right = _tree_remove(&tree->right, min_right->data, size, compare, delete);
        }
    }

    if (tree == NULL) {
        return NULL; // Si l'arbre est devenu vide, retourne NULL
    }

    // Met à jour le facteur d'équilibre et effectue les rotations si nécessaire
    tree->balance = tree_height(tree->left) - tree_height(tree->right);

    if (tree->balance > 1) {
        // Déséquilibre à gauche
        if (tree_height(tree->left->left) >= tree_height(tree->left->right)) {
            // Rotation simple à droite
            rotation_right(tree);
        } else {
            // Rotation double à gauche-droite
            rotation_left(tree->left);
            rotation_right(tree);
        }
    } else if (tree->balance < -1) {
        // Déséquilibre à droite
        if (tree_height(tree->right->right) >= tree_height(tree->right->left)) {
            // Rotation simple à gauche
            rotation_left(tree);
        } else {
            // Rotation double à droite-gauche
            rotation_right(tree->right);
            rotation_left(tree);
        }
    }

    return tree;
}

