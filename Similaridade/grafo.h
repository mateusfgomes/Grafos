#ifndef GRAFO_H
#define GRAFO_H
#define MAXNUMVERTICES 100
#define SEM_ARESTA 0

typedef int elem;
typedef struct grafo_ Grafo;

typedef struct no_aresta noAresta;
typedef struct no_vertice noVertice;

Grafo* criar_grafo(int* nVertices, int* erro);
void inserir_aresta(Grafo *G, int *v1, int *v2, elem *P, int* erro);
void remover_aresta(Grafo *G, int *v1, int *v2, int *erro, elem *P);
void imprime_grafo(Grafo *G);
double compara_grafos(Grafo* G1, Grafo* G2, int n_arestas1, int n_arestas2);


#endif
