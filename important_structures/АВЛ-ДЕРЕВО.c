#include <stdlib.h>
#include <stdio.h>
// меня никто не видит
typedef struct TreeNode {
    int key;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode *create_test_tree(void) {
    TreeNode *root = calloc(1, sizeof(TreeNode));
    root->key = 2;
    TreeNode *left = calloc(1, sizeof(TreeNode));
    left->key = 1;
    TreeNode *right = calloc(1, sizeof(TreeNode));
    right->key = 4;
    TreeNode *rright = calloc(1, sizeof(TreeNode));
    rright->key = 6;
    root->left = left;
    root->right = right;
    right->right = rright;
    return root;
}

void visit(TreeNode *node) {
    if (node) {
        visit(node->left);
        printf("%d ", node->key);
        visit(node->right);
    }
}

void free_tree(TreeNode *node) {
    if (node) {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}

TreeNode *search(TreeNode *node, int key) {
    if (node == NULL)
        return NULL;
    if (node->key == key)
        return node;
    if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

TreeNode *add (TreeNode *head, int n){
    if(!head){
        TreeNode *tmp = calloc(1, sizeof(TreeNode));
        tmp->key = n;
        return tmp;
    }
    if(n > head->key){
        head->right = add(head->right, n);
        return head;
    }
    else{
        head->left = add(head->left, n);
        return head;
    }
}

void add1(TreeNode **head, int key){
    if (!(*head)){
        TreeNode *tmp = calloc(1, sizeof(TreeNode));
        tmp->key = key;
        *head = tmp;
        return;
    }
    if (key > (*head)->key)
        add1(&((*head)->right), key);
    else
        add1(&((*head)->left), key);
    return;
}

TreeNode *get_min(TreeNode *node)
{
    if(!node)
        return NULL;
    if(node->left)
        return get_min(node->left);
    else
        return node;
}

int main(void) {
    TreeNode *tree = create_test_tree();
    visit(tree);
    add1(&tree, 0);
    printf("\n");
    add1(&tree, 3);
    printf("\n");
    visit(tree);
    //printf("\n%d\n", search(tree, 6)->key);
    //printf("%d ", get_min(tree)->key);
    //free_tree(tree);
    return 0;
}
// я крабик