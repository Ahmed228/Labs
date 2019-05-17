#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h.>
struct node {
    int key;
    char height;
    struct node* left;
    struct node* right;
};

char height (struct node* Tree){ //возвращает высоту дерева в данном узле
    if (Tree == 0) return 0;
    else return Tree->height;
}

int balancefactor (struct node* Tree){
    return height(Tree->right) - height(Tree->left);
}

void fixheight (struct node* Tree){ //функция, считающая высоту дерева в данном узле
    char h_l = height(Tree->left);
    char h_r = height(Tree->right);
    if (h_l > h_r) Tree->height = h_l + 1;
    else Tree->height = h_r + 1;
}

struct node* LeftRotate(struct node* q){ //левый поворот
    struct node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

struct node* RightRotate(struct node* p){ //правый поворот
    struct node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

struct node* BalanceTree (struct node* Tree){ //балансировка дерева
    fixheight(Tree);
    if (balancefactor(Tree) == 2){
        if (balancefactor(Tree->right) < 0) Tree->right = RightRotate(Tree->right);
        return LeftRotate(Tree);
    } else if (balancefactor(Tree) == -2){
        if (balancefactor(Tree->left) > 0) Tree->left = LeftRotate(Tree->left);
        return RightRotate(Tree);
    }
    return Tree;
}

struct node* NewNode (struct node* root, int k){ //добавление нового узла
    if (!root){
        root = malloc(sizeof(struct node));
        root->key = k;
        root->left = NULL;
        root->right = NULL;
        root->height = 1;
        return root;
    } else if (k > root->key) root->right = NewNode(root->right, k);
           else root->left = NewNode(root->left,k);
    return BalanceTree(root);
}

void free_tree (struct node* Tree){
    if (Tree->right) free_tree(Tree->right);
    if (Tree->left) free_tree(Tree->left);
    free(Tree);
}

int main(){
    struct node* root = NULL;
    int N = 0; int k = 0; int i = 0;
    scanf ("%d", &N);
    if (N == 0){
        printf ("0");
        return 0;
    } else for (i = 0; i < N; i++){
        scanf ("%d", &k);
        root = NewNode(root,k);
    }
    printf ("%d", height(root));
    free_tree(root);
    return 0;
}
