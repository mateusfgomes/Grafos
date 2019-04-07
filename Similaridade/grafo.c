#include "grafo.h"
#include <stdlib.h>
#include <stdio.h>


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
		G->Adj[*v2].fim->prox = (noAresta*) malloc(sizeof(noAresta));
		G->Adj[*v2].fim = G->Adj[*v2].fim->prox;
		G->Adj[*v2].fim->v = *v1;									//GRAFO DIRECIONADO
		G->Adj[*v2].fim->peso = *P;
		G->Adj[*v2].fim->prox = NULL;
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

void imprime_grafo(Grafo *G){

	noAresta* aux;
	for(int i = 0; i < G->NumVertices; i++){
		printf("%d - ", i);
		aux = G->Adj[i].ini->prox;
		while(aux != NULL){
			printf("%d ", aux->v);
			aux = aux->prox;
		}
		printf("\n");
	} 



}


double compara_grafos(Grafo* G1, Grafo* G2, int n_arestas1, int n_arestas2){

	double diferenca = abs(G1->NumVertices - G2->NumVertices);
	double conta;
	double retorno;

	if(G1->NumVertices != 0 && G2->NumVertices != 0){
		conta = diferenca/(G1->NumVertices + G2->NumVertices);
	}
	retorno = 50*(1-((conta)*(conta)));
    
	diferenca = abs(n_arestas1 - n_arestas2);

	if(n_arestas1 != 0 && n_arestas2 != 0){
		conta = diferenca/(n_arestas1 + n_arestas2);
	}
	retorno += 50*(1-((conta)*(conta)));

	return retorno/100;
}