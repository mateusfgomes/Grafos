#include "grafo.h"

int main(void){

    Grafo* gr;
    int zero = 0;
    int um = 1;
    int dois = 2;
    int tres = 3;
    int quatro = 4;
    int cinco = 5;
    int seis = 6;
    int sete = 7;
    int oito = 8;
    int peso = 0;

    gr = criar_grafo(&oito);

    inserir_aresta(gr, &zero, &um, &peso);
    inserir_aresta(gr, &zero, &dois, &peso);
    inserir_aresta(gr, &um, &tres, &peso);
   
    inserir_aresta(gr, &dois, &quatro, &peso);
    inserir_aresta(gr, &dois, &tres, &peso);
    inserir_aresta(gr, &tres, &quatro, &peso);
    inserir_aresta(gr, &tres, &cinco, &peso);

    inserir_aresta(gr, &quatro, &cinco, &peso);
    inserir_aresta(gr, &seis, &dois, &peso);
    inserir_aresta(gr, &seis, &sete, &peso);
    imprime_grafo(gr);

    bfs(gr);



    return 0;
}