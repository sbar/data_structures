#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"

BinTree *createBinTree()
{
    BinTree *tree = (BinTree *)malloc(sizeof(BinTree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for a binary tree\n");
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;

    return tree;
}

BinNode *createBinNode(int data, BinNode *parent, BinNode *left, BinNode *right)
{
    BinNode *node = (BinNode *)malloc(sizeof(BinNode));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory for a binary node\n");
        return NULL;
    }

    node->data = data;
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

/*void insert1(int data, BinNode **node, BinNode *parent)
{
    if (node == NULL)
        return;

    if (*node == NULL) {
        *node = createBinNode(data, parent, NULL, NULL);
        return;
    }

    if (data < (*node)->data)
        insert1(data, &((*node)->left), *node);
    else
        insert1(data, &((*node)->right), *node);
}

void insertBinNodeRecursive(int data, BinTree *tree)
{
    BinNode **root = &(tree->root);

    if (root == NULL)
        return;

    if (*root == NULL) {
        *root = createBinNode(data, NULL, NULL, NULL);
        if (*root == NULL)
            return;
    } else {
        insert1(data, root, (*root)->parent);
    }

    tree->size++;
}*/

void insertBinNode(int data, BinTree *tree)
{
    if (tree->root == NULL) {
        tree->root = createBinNode(data, NULL, NULL, NULL);
    } else {
        BinNode *node = tree->root, *parent = NULL;
        while (node != NULL) {
            parent = node;
            node = node->data > data ? node->left : node->right;
        }
        if (parent->data > data)
            parent->left = createBinNode(data, parent, NULL, NULL);
        else
            parent->right = createBinNode(data, parent, NULL, NULL);
    }
    tree->size++;
}

/*void traversalPreorderRecursive(BinNode *root, void (*f)(BinNode *, int), int level)
{
    f(root, level);

    if (root == NULL)
        return;

    traversalPreorderRecursive(root->left, f, level + 1);
    traversalPreorderRecursive(root->right, f, level + 1);
}

void traversalInorderRecursive(BinNode *root, void (*f)(BinNode *, int), int level)
{
    if (root != NULL)
        traversalInorderRecursive(root->left, f, level + 1);
    
    f(root, level);

    if (root != NULL)
        traversalInorderRecursive(root->right, f, level + 1);
}

void traversalPostorderRecursive(BinNode *root, void (*f)(BinNode *, int), int level)
{
    if (root != NULL)
        traversalPostorderRecursive(root->left, f, level + 1);
    
    if (root != NULL)
        traversalPostorderRecursive(root->right, f, level + 1);

    f(root, level);
}*/

/*void traversalInorder(BinNode *root, void (*f)(BinNode *, int), int level)
{
    Stack *s = createStack();
    Pair *p;
    BinNode *node = root;
    level = -1;

    for (;;) {
        if (node != NULL) {
            p = createPair(node, level + 1);
            pushStack(s, p);
            free(p);
            node = node->left;
            ++level;
            continue;
        } else {
            f(NULL, level + 1);
        }
        
        if (isEmptyStack(s))
            break;

        p = popStack(s);
        node = p->first;
        level = p->second;
        free(p);

        f(node, level);

        node = node->right;
    }
    freeStack(&s);
}*/

void traversalPreorder(BinNode *root, void (*f)(BinNode *, int))
{
    Stack *s = createStack();
    Pair *p = createPair(root, 0);
    pushStack(s, p);
    free(p);

    while (!isEmptyStack(s))
    {
        p = popStack(s);
        BinNode *node = p->first;
        int level = p->second;
        free(p);

        f(node, level);

        if (node->right != NULL) {
            p = createPair(node->right, level + 1);
            pushStack(s, p);
            free(p);
        } else {
            f(NULL, level + 1);
        }
        if (node->left != NULL) {
            p = createPair(node->left, level + 1);
            pushStack(s, p);
            free(p);
        } else {
            f(NULL, level + 1);
        }
    }
    freeStack(&s);
}

void traversalInorder(BinNode *root, void (*f)(BinNode *, int))
{
    Stack *s = createStack();
    Pair *p;
    BinNode *node = root;
    int level = -1;

    while (!isEmptyStack(s) || node != NULL)
    {
        if (node != NULL) {
            p = createPair(node, level + 1);
            pushStack(s, p);
            node = node->left;
            free(p);
            ++level;
        } else {
            f(NULL, level + 1);
            p = popStack(s);
            node = p->first;
            level = p->second;
            free(p);
            f(node, level);
            node = node->right;
        }
    }
    f(NULL, level + 1);
    freeStack(&s);
}

void traversalPostorder(BinNode *root, void (*f)(BinNode *, int))
{
    Stack *s = createStack();
    BinNode *node = root;
    int level = 0;
    Pair *p;

    while (node != NULL || !isEmptyStack(s))
    {
        if (node == NULL) {
            f(NULL, level);
            if (s->head->data.first->right == NULL)
                f(NULL, level);
            while (!isEmptyStack(s) && s->head->data.first->right == node) {
                p = popStack(s);
                node = p->first;
                level = p->second;
                free(p);
                f(node, level);
            }
            node = isEmptyStack(s) ? NULL : s->head->data.first->right;
        }
        if (node != NULL) {
            ++level;
            p = createPair(node, level - 1);
            pushStack(s, p);
            free(p);
            node = node->left;
        }
    }
    freeStack(&s);
}

void traversalLevelorder(BinNode *root, void (*f)(BinNode *, int))
{
    Queue *q = createQueue();
    Pair *p = createPair(root, 0);
    pushQueue(q, p);
    free(p);
    while (!isEmptyQueue(q)) {
        p = popQueue(q);
        BinNode *node = p->first;
        int level = p->second;
        f(node, level);
        free(p);
        if (node->left != NULL) {
            p = createPair(node->left, level + 1);
            pushQueue(q, p);
            free(p);
        } else {
            f(NULL, level + 1);
        }
        if (node->right != NULL) {
            p = createPair(node->right, level + 1);
            pushQueue(q, p);
            free(p);
        } else {
            f(NULL, level + 1);
        }
    }
    freeQueue(&q);
}

/*BinNode *search(BinNode *root, int data)
{
    if (root == NULL || root->data == data)
        return root;

    if (root->data > data)
        return search(root->left, data);
    else
        return search(root->right, data);
}*/

BinNode *search(BinNode *root, int data)
{
    while (root != NULL && root->data != data) {
        if (root->data > data)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

BinNode *findMin(BinNode *node)
{
    if (node == NULL)
        return NULL;

    while (HAS_LEFT(node))
        node = node->left;
    return node;
}

// updates parent's pointer before removal
void updateParent(BinNode **node, BinNode *value)
{
    if (node == NULL)
        return;

    if ((*node)->parent == NULL)
        return;

    if ((*node)->parent->left == *node)
        (*node)->parent->left = value;
    else
        (*node)->parent->right = value;

    if (value != NULL)
        value->parent = (*node)->parent;
}

void removeBinNode(BinNode **node, BinTree *tree)
{
    if (node == NULL || *node == NULL)
        return;

    if (IS_LEAF(*node)) {
        // 1. Deleting a leaf (node with no children):
        // Deleting a leaf is easy, as we can simply remove it from the tree.
        updateParent(node, NULL);
        free((void *)*node);
    } else if (HAS_LEFT(*node) && !HAS_RIGHT(*node)) {
        // 2. Deleting a node with one child:
        // Remove the node and replace it with its child.
        updateParent(node, (*node)->left);
        free((void *)*node);
    } else if (!HAS_LEFT(*node) && HAS_RIGHT(*node)) {
        updateParent(node, (*node)->right);
        free((void *)*node);
    } else {  //  HAS_LEFT(*node) && HAS_RIGHT(*node)
        // 3. Deleting a node with two children:
        // Do not delete "node". Find the smallest (the leftest) child of
        // the right node's subtree (call him "m") and copy from "m" to "node"
        // all data (not including left, right or parent pointers) to "node".
        // Remove m.
        BinNode *m = findMin((*node)->right);
        (*node)->data = m->data;
        removeBinNode(&m, tree);
    }

    tree->size--;
}

void removeBinNodeByData(int data, BinTree *tree)
{
    BinNode *f = search(tree->root, data);

    if (f == NULL)
        fprintf(stderr, "Unable to find node %d\n", data);
    else
        removeBinNode(&f, tree);
}

void printBinNode(BinNode *node, int level)
{
    int i;
    //for (i = 0; i < level * TABWIDTH; ++i)
    for (i = level * TABWIDTH; i > 0; --i)
        printf(" ");

    if (node == NULL)
        printf("NULL\n");
    else
        printf("%d\n", node->data);
}

void printBinTree(BinTree *tree)
{
    printf("\nPreorder traversal:\n");
    traversalPreorder(tree->root, &printBinNode);
}

void printBinTreeByLevel(BinTree *tree)
{
    printf("\nQueue-based level order traversal:\n");
    traversalLevelorder(tree->root, &printBinNode); 
}

void freeBinNode(BinNode *node, int level)
{
    if (node != NULL)
        free((void *)node);
}

void clearBinTree(BinTree *tree)
{
    if (tree == NULL || tree->root == NULL || tree->size == 0)
        return;

    traversalPostorder(tree->root, &freeBinNode);
    tree->size = 0;
}

void freeBinTree(BinTree **tree)
{
    clearBinTree(*tree);
    free((void *)*tree);
    tree = NULL;
}

int isEmptyTree(BinTree *tree)
{
    return tree->size == 0;
}
