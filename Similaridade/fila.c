#include "fila.h"
#include <stdlib.h>

struct no_{
    int elem;
    NO* prox;
};


struct fila_{
    int tamanho;
    NO* inicio;
    NO* fim;
};

FILA* criar_fila(){

    FILA* f = (FILA*) malloc(sizeof(FILA));

    if(f != NULL)
        return f;
    
    else
        return NULL;

}

int entra(FILA* f, int insere){

    NO* aux = (NO*) malloc(sizeof(NO));
    aux->elem = insere;

    if(f == NULL)
        return 0;

    if(f->tamanho != 0) 
        f->fim->prox = aux;
    
    f->fim = aux;
    
    if(f->tamanho == 0)
        f->inicio = aux;
    
    f->tamanho++;
    
    return f->tamanho;

}

int sai(FILA* f){

    NO* frente;
    NO* tras;
    int i = 0;

    frente = f->inicio;

    if(f == NULL)
        return -545;


    while(i < f->tamanho - 1){
        tras = frente;
        frente = frente->prox;
        i++;
    }

    f->fim = tras;
    if(f->tamanho == 1)
        f->inicio = f->fim;
    
    f->tamanho--;

    return frente->elem;

}

 
int fila_vazia(FILA* f){
    if(f->tamanho == 0)
        return 1;
    else
        return 0;
}