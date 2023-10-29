#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "tree-avl.h"
#include "min-max.h"

/*
 * Retourne un nouvel arbre vide.
 */
Tree tree_new() {
    return NULL;
}

/*
 * Supprime l'arbre passé en paramètre.
 */
void tree_delete(Tree tree, void (*delete)(void *)) {
    if (tree) {
        tree_delete(tree->left, delete);
        tree_delete(tree->right, delete);
        if (delete)
            delete(tree->data);
        free(tree);
    }
}

/*
 * Crée un arbre avec une valeur passée en paramètre.
 */
Tree tree_create(const void *data, size_t size) {
    Tree tree = (Tree) malloc(3 * sizeof(Tree) + size + sizeof(size_t));

    if (tree) {
        tree->left = NULL;
        tree->right = NULL;
        tree->parent = NULL;
        memcpy(tree->data, data, size);
        tree->balance = 0;
    }

    return tree;
}

/*
 * Retourne le fils gauche de l'arbre passé en paramètre.
 */
Tree tree_get_left(Tree tree) {
    if (tree)
        return tree->left;
    else
        return NULL;
}

/*
 * Retourne le fils droit de l'arbre passé en paramètre.
 */
Tree tree_get_right(Tree tree) {
    if (tree)
        return tree->right;
    else
        return NULL;
}

/*
 * Retourne la valeur du nœud passé en paramètre.
 */
void *tree_get_data(Tree tree) {
    if (tree)
        return tree->data;
    else
        return NULL;
}

/*
 * Définit le fils gauche de l'arbre passé en paramètre.
 */
bool tree_set_left(Tree tree, Tree left) {
    if (tree) {
        tree->left = left;
        left->parent = tree;
        return true;
    } else
        return false;
}

/*
 * Définit le fils droit de l'arbre passé en paramètre.
 */
bool tree_set_right(Tree tree, Tree right) {
    if (tree) {
        tree->right = right;
        right->parent = tree;
        return true;
    } else
        return false;
}

/*
 * Redéfinit la valeur du nœud passé en paramètre.
 */
bool tree_set_data(Tree tree, const void *data, size_t
size) {
    if (tree) {
        memcpy(tree->data, data, size);
        return true;
    } else
        return false;
}

/*
 * Parcours l'arbre passé en paramètre en mode VGD.
 */
void tree_pre_order(Tree tree,
                    void (*func)(void *, void *),
                    void *extra_data) {
    if (tree) {
        func(tree->data, extra_data);
        tree_pre_order(tree->left, func, extra_data);
        tree_pre_order(tree->right, func, extra_data);
    }
}

/*
 * Parcours l'arbre passé en paramètre en mode GVD.
 */
void tree_in_order(Tree tree,
                   void (*func)(void *, void *),
                   void *extra_data) {
    if (tree) {
        tree_in_order(tree->left, func, extra_data);
        func(tree->data, extra_data);
        tree_in_order(tree->right, func, extra_data);
    }
}

/*
 * Parcours l'arbre passé en paramètre en mode GDV.
 */
void tree_post_order(Tree tree,
                     void (*func)(void *, void *),
                     void *extra_data) {
    if (tree) {
        tree_post_order(tree->left, func, extra_data);
        tree_post_order(tree->right, func, extra_data);
        func(tree->data, extra_data);
    }
}

/*
 * Retourne la hauteur (maximale) de l'arbre passé en paramètre.
 */
size_t tree_height(Tree tree){
  if (tree)
    return 1 + MAX(tree_height(tree->left), tree_height(tree->right));
  else
    return 0;
}

/*
 * Retourne le nombre de nœuds de l'arbre passé en paramètre.
 */
size_t tree_size(Tree tree) {
    if (tree)
        return 1 + tree_size(tree->left) + tree_size
                (tree->right);
    else
        return 0;
}

/*
 * (NE PAS CONSIDÉRER. UNIQUEMENT utilisé par tree_sort(). Voir tree_insert() pour les insertions réellement effectuées.)
 * Insère une valeur dans l'arbre passé en paramètre, de manière triée.
 */
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

/*
 * Retourne le nœud dont la valeur correspond à celle passée en paramètre, sinon NULL.
 */
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
    } else {
        return NULL;
    }
}

/*
 * (NE PAS CONSIDÉRER)
 * (Utilisée par tree_sort)
 */
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

/*
 * (NE PAS CONSIDÉRER)
 * Crée un arbre trié à partir d'un tableau de valeurs.
 */
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

/*
 * Effectue une rotation à gauche de l'arbre passé en paramètre.
 */
void rotation_left(Tree tree) {
    if (tree->right == NULL) return;

    Tree treeParent = tree->parent;
    Tree newParent = tree->right;
    Tree children = tree;

    if (treeParent) {
        if (compare(tree->data, treeParent->data) > 0) {
            treeParent->right = newParent;
        } else {
            treeParent->left = newParent;
        }
    }

    newParent->left = children;
    children->parent = newParent;
    children->right = NULL;

    tree->balance = tree_height(tree->left) - tree_height(tree->right);
    newParent->balance = tree_height(newParent->left) - tree_height(newParent->right);
}

/*
 * Effectue une rotation à droite de l'arbre passé en paramètre.
 */
void rotation_right(Tree tree) {
    if (tree->left == NULL) return;

    Tree treeParent = tree->parent;
    Tree newParent = tree->left;
    Tree children = tree;

    if (treeParent) {
        if (compare(tree->data, treeParent->data) > 0) {
            treeParent->right = newParent;
        } else {
            treeParent->left = newParent;
        }
    }

    newParent->right = children;
    children->parent = newParent;
    children->left = NULL;

    tree->balance = tree_height(tree->left) - tree_height(tree->right);
    newParent->balance = tree_height(newParent->left) - tree_height(newParent->right);
}

/*
 * Équilibre l'arbre passé en paramètre (si nécessaire).
 */
void balance(Tree *ptree) {
    if (*ptree) {
        const void *data = (*ptree)->data; // Déclarer et initialiser data avec la valeur du nœud
        int balance = tree_height((*ptree)->left) - tree_height((*ptree)->right);

        if ((int) balance > 1) {
            // Déséquilibre à gauche
            if (compare(data, (*ptree)->left->data) < 0) {
                // Rotation simple à droite
                rotation_right(*ptree);
            } else {
                // Rotation double à gauche-droite
                rotation_left((*ptree)->left);
                rotation_right(*ptree);
            }
        } else if ((int) balance < -1) {
            // Déséquilibre à droite
            if (compare(data, (*ptree)->right->data) > 0) {
                // Rotation simple à gauche
                rotation_left(*ptree);
            } else {
                // Rotation double à droite-gauche
                rotation_right((*ptree)->right);
                rotation_left(*ptree);
            }
        }
    }
}

/*
 * Insère une valeur dans l'arbre passé en paramètre, tout en conservant les propriétés d'un arbre AVL.
 */
Tree tree_insert(Tree *ptree, const void *data, size_t size, int (*compare)(const void *, const void *)) {
    if (*ptree == NULL) {
        // Créez un nouveau nœud pour contenir les données.
        Tree new = tree_create(data, size);

        if (new) {
            *ptree = new;  // Ajoutez le nœud créé à l'arbre.
        }
    } else {
        int C = compare(data, (*ptree)->data);

        if (C < 0) {
            // Insérez à gauche et mettez à jour l'arbre équilibré.
            (*ptree)->left = tree_insert(&((*ptree)->left), data, size, compare);
            (*ptree)->left->parent = *ptree;

            // Équilibrer l'arbre
            balance(ptree);
        } else if (C > 0) {
            // Insérez à droite et mettez à jour l'arbre équilibré.
            (*ptree)->right = tree_insert(&((*ptree)->right), data, size, compare);
            (*ptree)->right->parent = *ptree;

            // Équilibrer l'arbre
            balance(ptree);
        }
    }

    return *ptree;
}

/*
 * Retourne la valeur minimum de l'arbre passé en paramètre.
 */
Tree tree_min(Tree tree) {
    if (tree == NULL) {
        return NULL;
    }

    while (tree->left != NULL) {
        tree = tree->left;
    }

    return tree;
}

/*
 * Supprime le nœud dont la valeur correspond à celle passée en paramètre (s'il existe), tout en conservant les propriétés d'un arbre AVL.
 */
Tree _tree_remove(Tree *ptree, const void *data, size_t size, int (*compare)(const void *, const void *), void (*delete)(void *)) {
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

            if (tree->parent) {
                Tree parent = tree->parent;

                if (compare(tree->data, parent->data) > 0) {
                    parent->right = temp;
                } else {
                    parent->left = temp;
                }
            }

            free(tree);
            tree = temp;
        } else if (tree->right == NULL) {
            Tree temp = tree->left;

            if (delete) {
                delete(tree->data);
            }

            if (tree->parent) {
                Tree parent = tree->parent;

                if (compare(tree->data, parent->data) > 0) {
                    parent->right = temp;
                } else {
                    parent->left = temp;
                }
            }

            free(tree);
            tree = temp;
        } else {
            // Cas où le nœud a deux sous-arbres

            Tree temp = tree_min(tree->right); // Trouver le nœud successeur
            memcpy(tree->data, temp->data, size);
            tree->right = _tree_remove(&tree->right, temp->data, size, compare, delete);
        }
    }

    if (tree == NULL) {
        return NULL; // Si l'arbre est devenu vide, retourne NULL
    }

    // Met à jour le facteur d'équilibre et effectue les rotations si nécessaire
    tree->balance = tree_height(tree->left) - tree_height(tree->right);

    if ((int) tree->balance > 1) {
        // Déséquilibre à gauche
        if (tree_height(tree->left->left) >= tree_height(tree->left->right)) {
            // Rotation simple à droite
            rotation_right(tree);
        } else {
            // Rotation double à gauche-droite
            rotation_left(tree->left);
            rotation_right(tree);
        }
    } else if ((int) tree->balance < -1) {
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