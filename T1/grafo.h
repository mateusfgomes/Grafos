#ifndef GRAFO_H
#define GRAFO_H
#define MAXNUMVERTICES 100
#define SEM_ARESTA 0

typedef int elem;
typedef struct grafo_ Grafo;

typedef struct no_aresta noAresta;
typedef struct no_vertice noVertice;

Grafo* criar_grafo(int* nVertices);
void inserir_aresta(Grafo *G, int *v1, int *v2, elem *P);
int numero_vertices(Grafo *G);
void remover_aresta(Grafo *G, int *v1, int *v2, int *erro, elem *P);
void imprime_grafo(Grafo *G);
double compara_grafos(Grafo* G1, Grafo* G2, int n_arestas1, int n_arestas2);
void dfs(Grafo *gr);
void dfs_busca(Grafo* gr, int inicial, int *cores);
int menor_peso(Grafo *gr, int vertice);
void n_arestas_recursivo(Grafo *gr, int inicio, int *contador);
int n_arestas_iterativo(Grafo *gr);
void bfs(Grafo* G);
void caminho(int u, int v, int antecessor[]);
void visita_bfs(Grafo* G, int v, int distancia[], int cor[], int antecessor[]);
void proxadj(Grafo *gr, noAresta **Adj, int *fimlistaadj);
noAresta* primeirolistaadj(Grafo *gr, int v);

#endif
