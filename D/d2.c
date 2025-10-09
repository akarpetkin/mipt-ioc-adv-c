#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct tree {
    datatype key;
    struct tree *left, *right;
    struct tree *parent;
} tree;

tree* createNode(datatype key, tree* parent) {
    tree* newNode = (tree*)malloc(sizeof(tree));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    newNode->parent = parent;
    return newNode;
}

tree* findNode(tree* root, int key) {
    if (root == NULL) return NULL;
    
    if (root->key == key) return root;
    
    tree* leftResult = findNode(root->left, key);
    if (leftResult != NULL) return leftResult;
    
    return findNode(root->right, key);
}

tree* findBrother(tree* root, int key) {
    tree* node = findNode(root, key);
    
    if (node == NULL) return 0;
    if (node->parent == NULL) return 0;
    
    tree* parent = node->parent;
    
    if (parent->left == node) {
        return parent->right;
    }

    else if (parent->right == node) {
        return parent->left; 
    }
    
    return 0;
}

int main() {

    tree* root = createNode(10, NULL);
    
    root->left = createNode(5, root);
    root->right = createNode(15, root);
    
    root->left->left = createNode(3, root->left);
    root->left->right = createNode(7, root->left);
    
    root->right->right = createNode(20, root->right);
    
    root->left->right->right = createNode(8, root->left->right);
    
    tree* brother;
    brother = findBrother(root, 5);
  
    return 0;
}