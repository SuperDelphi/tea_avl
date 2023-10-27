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
// Fonction pour équilibrer un nœud
void balance(Tree *ptree) {
    int balance = tree_height((*ptree)->left) - tree_height((*ptree)->right);

    if (balance > 1) {
        // Déséquilibre à gauche
        if (compare(data, (*ptree)->left->data) < 0) {
            // Rotation simple à droite
            *ptree = rotation_right(*ptree);
        } else {
            // Rotation double à gauche-droite
            (*ptree)->left = rotation_left((*ptree)->left);
            *ptree = rotation_right(*ptree);
        }
    } else if (balance < -1) {
        // Déséquilibre à droite
        if (compare(data, (*ptree)->right->data) > 0) {
            // Rotation simple à gauche
            *ptree = rotation_left(*ptree);
        } else {
            // Rotation double à droite-gauche
            (*ptree)->right = rotation_right((*ptree)->right);
            *ptree = rotation_left(*ptree);
        }
    }
}

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

// Fonction pour insérer un nœud
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

            // Équilibrer l'arbre
            balance(ptree);
        } else if (C > 0) {
            // Insérez à droite et mettez à jour l'arbre équilibré.
            (*ptree)->right = tree_insert(&((*ptree)->right), data, size, compare);

            // Équilibrer l'arbre
            balance(ptree);
        }
    }

    return *ptree;
}



Tree tree_min(Tree tree) {
    if (tree == NULL) {
        return NULL;
    }

    while (tree->left != NULL) {
        tree = tree->left;
    }

    return tree;
}

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
            tree = temp;
        } else if (tree->right == NULL) {
            Tree temp = tree->left;
            if (delete) {
                delete(tree->data);
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
            tree = temp;
        } else if (tree->right == NULL) {
            Tree temp = tree->left;
            if (delete) {
                delete(tree->data);
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

    // Met à jour le facteur d'équilibre
    tree->balance = tree_height(tree->left) - tree_height(tree->right);

    // Équilibrer l'arbre
    balance(ptree);

    return tree;
}
