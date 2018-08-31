/*
Search Tree
*/


#include <stdbool.h>

typedef struct Node *Tree;

typedef struct Node {
   Tree left, right;
   char  data[100];
   int *url;
   int count;
} Node;

typedef char *Item;      // item is just a key

Tree newTree();        // create an empty Tree
void freeTree(Tree);   // free memory associated with Tree
void showTree(Tree);   // display a Tree (sideways)
void showcurrent(Tree t);   //show the info of current node

Tree TreeSearch(Tree, Item);   // check whether an item is in a Tree
Tree TreeInsert(Tree t, char *word, int url);   // insert a new item into a Tree

void TreeInfix(Tree); //print in infix order




