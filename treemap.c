#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    if (tree == NULL || tree->root == NULL) 
    {
        tree->root = createTreeNode(key, value);
        return;
    }
    if (searchTreeMap(tree, key) == NULL)
    {
        TreeNode * aux = tree->root;
        TreeNode * parent = NULL;
        while (aux != NULL)
        {
            parent = aux;
            if (tree->lower_than(key, aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
        }
        TreeNode * nodo = createTreeNode(key, value);
        nodo->parent = parent;
        if (tree->lower_than(key, parent->pair->key) == 1)
        {
            parent->left = nodo;
            
        }
        else{
            parent->right = nodo;
        }
        tree->current = nodo;
        
        
        
    }
    return;
    

}

TreeNode * minimum(TreeNode * x){
    if(x->left == NULL) return x;
    TreeNode *aux = x->left;
    while(aux->left != NULL)
        {
            aux = aux->left;
        }
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || tree->root == NULL) return;
    if (node == NULL) return;
    TreeNode *aux = tree->root;
    while(aux != node)
        {
            if(tree->lower_than(node->pair->key,aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
            
        }
    TreeNode *parent = aux->parent;
    if (aux->left == NULL && aux->right == NULL)
    {
        if(is_equal(tree, parent->left->pair,aux->pair) == 1)
        {
            parent->left = NULL;
            free(aux);
            
            return;
        }
        if(is_equal(tree, parent->right->pair,aux->pair) == 1)
        {
            parent->right = NULL;
            free(aux);
            
            return;
        }
    }
    else if((aux->left == NULL && aux->right != NULL) || (aux->left != NULL && aux->right == NULL))
    {
        if(is_equal(tree, parent->left->pair,aux->pair) == 1)
        {
            if(aux->left == NULL)
            {
                parent->left = aux->right;
                aux->right->parent = parent;
                free(aux);
                return;
            }
            else
            {
                parent->left = aux->left;
                aux->left->parent = parent;
                free(aux);
                return;
            }
        }
        else
        {
            if(aux->left == NULL)
            {
                parent->right = aux->right;
                aux->right->parent = parent;
                free(aux);
                return;
            }
            else
            {
                parent->right = aux->left;
                aux->left->parent = parent;
                free(aux);
                return;
            }
        }
    }
    else
    {
        TreeNode *min = minimum(aux->right);
        aux->pair->key = min->pair->key;
        aux->pair->value = min->pair->value;
        removeNode(tree, min);
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                tree->current = aux;
                return aux->pair;
            }
            if (tree->lower_than(key, aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
            
        }
    return NULL;

    
    
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    TreeNode * ub = NULL;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                ub = aux;
                break;
            }
            if (tree->lower_than(key, aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                ub = aux;
            }
        }
    if (ub == NULL) return NULL;
    return ub->pair;
    
}

Pair * firstTreeMap(TreeMap * tree) {
    
    TreeNode *minNode = minimum(tree->root);
    if(minNode != NULL)
    {
        return minNode->pair;
    }
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->current;
    if(aux->right != NULL)
    {
        tree->current = minimum(aux->right);
        return tree->current->pair;
    }
    TreeNode *parent = aux->parent;
    while(parent != NULL && aux == parent->right)
        {
            aux = parent;
            parent = parent->parent;
        }
    tree->current = parent;
    if(parent != NULL)
    {
        return parent->pair;
    }
    return NULL;
    
}
