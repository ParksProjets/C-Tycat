/*

A simple source code.

Copyright (C) 2018, Guillaume Gonnet
This project is under the MIT License

*/

#include <stdio.h>
#include <stdlib.h>


#define debugger asm volatile ("int3;")


// A node in the binary tree
struct node {
    int value;
    struct node *left;
    struct node *right;
};


// A simple binary tree.
struct tree {
    struct node *root;
};



// Insert a new element in the binary tree.
void insert(struct tree *tree, int value)
{
    struct node **curs = &tree->root;
    while (*curs)
        curs = (value <= (*curs)->value) ? &(*curs)->left : &(*curs)->right;

    *curs = malloc(sizeof(struct node *));
    **curs = (struct node) { value };
}



int main()
{
    struct tree tree = {0};

    insert(&tree, 12);
    insert(&tree, 5);
    insert(&tree, 20);
    insert(&tree, 2);

    debugger;

    insert(&tree, 10);
    insert(&tree, 14);

    debugger;
}
