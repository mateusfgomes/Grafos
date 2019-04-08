#include "grafo.h"
#include <stdlib.h>


struct no_aresta{
	int v;
	elem peso;
	noAresta *prox;

};

struct no_vertice{
	noAresta *ini, *fim;
};

struct grafo_{
	noVertice Adj[MAXNUMVERTICES];
	int NumVertices;
};


Grafo* criar_grafo(int* NumVertices, int* erro){

	if(*NumVertices > MAXNUMVERTICES){
		*erro = 1;
		return NULL;
	}
	else{
		Grafo* G = (Grafo *) malloc(sizeof(Grafo));
		if(!G){
			*erro = 1;
		}
		else{
			*erro = 0;
			G->NumVertices = *NumVertices;
			for(int i = 0; i < G->NumVertices; i++){
				G->Adj[i].ini = (noAresta*) malloc(sizeof(noAresta));
				G->Adj[i].ini->peso = -1;
				G->Adj[i].ini->v = -1;
				G->Adj[i].ini->prox = NULL;
				G->Adj[i].fim = G->Adj[i].ini;
			}
		}
		return(G);
	}
}

void inserir_aresta(Grafo *G, int *v1, int *v2, elem *P, int *erro){
	if((*v1 >= G->NumVertices) || (*v2 >= G->NumVertices))
		*erro = 1;
	else{
		*erro = 0;
		G->Adj[*v1].fim->prox = (noAresta*) malloc(sizeof(noAresta));
		G->Adj[*v1].fim = G->Adj[*v1].fim->prox;
		G->Adj[*v1].fim->v = *v2;									//GRAFO DIRECIONADO
		G->Adj[*v1].fim->peso = *P;
		G->Adj[*v1].fim->prox = NULL;
	}
}

void remover_aresta(Grafo *G, int *v1, int *v2, int *erro, elem *P){
	if((*v1 >= G->NumVertices) || (*v2 >= G->NumVertices))
		*erro = 1;
	else{
		*erro = 0;
		int encontrou = 0;
		noAresta *ant = G->Adj[*v1].ini;
		noAresta *atual = ant->prox;
		while(atual != NULL && !encontrou){
			if(atual->v == *v2){
				encontrou = 1;
				*P = atual->peso;
				ant->prox = atual->prox;
				if(G->Adj[*v1].fim == atual){
					G->Adj[*v1].fim = ant;
					free(atual);
				}
				else{
					ant = atual;
					atual = atual->prox;
				}
			}
		}
	}

}
