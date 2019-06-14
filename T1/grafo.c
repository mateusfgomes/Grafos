#include "grafo.h"
#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

#define BRANCO 0
#define AMARELO 1
#define VERMELHO 2


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


Grafo* criar_grafo(int* NumVertices){

	if(*NumVertices > MAXNUMVERTICES){
		return NULL;
	}
	else{
		Grafo* G = (Grafo *) malloc(sizeof(Grafo));
		if(!G){
			return NULL;
		}
		else{
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

void inserir_aresta(Grafo *G, int *v1, int *v2, elem *P){
	if((*v1 >= G->NumVertices) || (*v2 >= G->NumVertices))
		return;
	else{
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

int numero_vertices(Grafo *G){
	return G->NumVertices;
}

void imprime_grafo(Grafo *G){

	noAresta* aux;
	for(int i = 0; i < G->NumVertices; i++){
		printf("%d - ", i);
		aux = G->Adj[i].ini->prox;
		while(aux != NULL){
			printf("(%d)%d ", aux->peso, aux->v);
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

void dfs_busca(Grafo* gr, int inicial, int *cores){

	inicial = AMARELO;
	noAresta* aux;
	int caminho[gr->NumVertices];

	aux = gr->Adj[inicial].ini;

	while(aux != NULL){
		if(cores[aux->v] == BRANCO){
			dfs_busca(gr, aux->v, cores);	
		}
		aux = aux->prox;
	}

	cores[inicial] = VERMELHO;

}

void dfs(Grafo *gr){

	int cores[gr->NumVertices];

	for(int i = 0; i < gr->NumVertices; i++){
		cores[i] = BRANCO;
	}

	for(int i = 0; i < gr->NumVertices; i++){
		if(cores[i] == BRANCO)
			dfs_busca(gr, i, cores);	
	}

}

int menor_peso(Grafo *gr, int vertice){

	noAresta *aux;

	int menor = 9999999;
	aux = gr->Adj[vertice].ini;	
	
	while(aux != NULL){
		if(aux->peso < menor){
			menor = aux->peso;
		}
		aux = aux->prox;
	}

	return menor;

}

int listaadjvazia(Grafo *gr, int v){

	if(gr->Adj[v].ini->prox == NULL)
		return 1;
	else
		return 0;

}

void n_arestas_recursivo(Grafo *gr, int inicio, int *contador){

	noAresta *aux;

	aux = gr->Adj[inicio].ini;

	while(aux != NULL){
		aux = aux->prox;
		(*contador)++;
	}
	n_arestas_recursivo(gr, aux->v, contador);
}

int n_arestas_iterativo(Grafo *gr){

	noAresta *aux;
	int contador = 0;

	for(int i = 0; i < gr->NumVertices; i++){
		aux = gr->Adj[i].ini;
		while(aux != NULL){
			aux = aux->prox;
			contador++;
		}
	}

	return contador;
}

noAresta* primeirolistaadj(Grafo *gr, int v){

	if(gr == NULL)
		return NULL;
		
	else
		return gr->Adj[v].ini->prox;

}


void proxadj(Grafo *gr, noAresta **Adj, int *fimlistaadj){

	//printf("- %d -", Adj->v);
	
}


void visita_bfs(Grafo* G, int v, int distancia[], int cor[], int antecessor[]){

	int peso, erro;
	noAresta *Adj;
	noVertice Aux;
	
	FILA *F;
	F = criar_fila();
	cor[v] = AMARELO;
	//distancia[v] = 0;
	entra(F, v);
	printf("v%d ", v);
	printf("\n----------- Fim Onda----------\n");
	while(!fila_vazia(F)){
		v = sai(F);
		if(!listaadjvazia(G, v)){
			Aux = G->Adj[v];
			Adj = Aux.ini->prox;
			while(Adj != NULL){
				if(cor[Adj->v] == BRANCO){
					printf("v%d ", Adj->v);
					cor[Adj->v] = AMARELO;
					entra(F, Adj->v);
				}
				Adj = Adj->prox;
			}
			printf("\n----------- Fim Onda----------\n");
		}
	}

}

void caminho(int u, int v, int antecessor[]){


	if(u == v)
		printf("%d", u);

	else if(antecessor[v] == -1)
		printf("ERRO");

	else{
		caminho(u, antecessor[v], antecessor);
		printf("%d", v);
	}
	

}

void bfs(Grafo* G){

	int v, distancia[MAXNUMVERTICES], antecessor[MAXNUMVERTICES];
	int cor[MAXNUMVERTICES];

	for(int v = 0; v < G->NumVertices; v++){
		cor[v] = BRANCO;
		//distancia[v] = -1;
		//antecessor[v] = -1;
	}

	for(int v = 0; v < G->NumVertices; v++){
		if(cor[v] == BRANCO)
			visita_bfs(G, v, distancia, cor, antecessor);
	}


}

