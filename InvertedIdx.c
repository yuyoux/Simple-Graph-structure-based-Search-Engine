/*
 Inverted Index ADT (partial) implementation, written by Ashesh Mahidadia Jan 2018.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "InvertedIdx.h"


#define data(tree)  ((tree)->data)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)



// make a new node containing data
Tree newNode(char *line) {
   Tree new = malloc(sizeof(Node));
   assert(new != NULL);
   strcpy(new->data, line);  
   left(new) = right(new) = NULL;
   new ->count = 1;
   new ->url = (int*)malloc(sizeof(int)); 
   return new;
}

// create a new empty Tree
Tree newTree() {
   return NULL;
}

// free memory associated with Tree
void freeTree(Tree t) {
   if (t != NULL) {
      freeTree(left(t));
      freeTree(right(t));
      free(t ->data);
      free(t);
   }
}

// display Tree sideways
void showTreeR(Tree t, int depth) {
   if (t != NULL) {
      showTreeR(right(t), depth+1);
      int i;
      for (i = 0; i < depth; i++)
	 putchar('\t');            // TAB character
      printf("%s--%d\n", data(t),t->count);
      showTreeR(left(t), depth+1);
   }
}

void showTree(Tree t) {
   showTreeR(t, 0);
}

void showcurrent(Tree t){
	printf("%s--%d\n", data(t),t->count);
}

// check whether a key is in a Tree
Tree TreeSearch(Tree t, char *word) {
   if (t == NULL)
      return NULL;

   else if (strcmp(word, data(t)) < 0)   //replace, else if (word < data(t))
      return TreeSearch(left(t), word);
   
   else if (strcmp(word, data(t)) > 0)  //replace, else if (word > data(t))
      return TreeSearch(right(t), word);

   else                                 // word == data(t)
      return t;
}

// insert a new item into a Tree
Tree TreeInsert(Tree t, char *word, int url) {
   if (t == NULL){
      t = newNode(word);
      t->url[0] = url;
      //printf("%d  _+02_   %s\n", t->url[t->count-1], t->data);
   }

   else if (strcmp(word, data(t)) < 0)   //replace, else if (word < data(t))
      left(t) = TreeInsert(left(t), word, url);

   else if (strcmp(word, data(t)) > 0)  //replace, else if (word > data(t))
      right(t) = TreeInsert(right(t), word, url);
   
   else if (strcmp(word, data(t)) == 0){ //if already exist, add url to array
      if (url != t->url[t->count-1]){ //avoid duplicated url
      	t->url = realloc(t->url, (t->count+1)*sizeof(int));
      	t->url[t->count] = url;
      	t->count ++;
      	//printf("%d  _+01_   %s\n", t->url[t->count-1], t->data);
      }
   }  
   return t;
}




