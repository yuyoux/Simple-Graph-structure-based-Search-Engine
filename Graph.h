// Graph ADT interface ... COMP9024 17s2
#include <stdbool.h>


typedef struct GraphRep *Graph;


typedef struct GraphRep {
   int  **edges;   // adjacency matrix
   int    nV;      // #vertices
   int    nE;      // #edges
} GraphRep;


// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex v;
   Vertex w;
} Edge;

Graph newGraph(int); // new graph with V vertices
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
bool  adjacent(Graph, Vertex, Vertex); // is there an edge between two vertices
void  showGraph(Graph);
void  freeGraph(Graph);
