#ifndef FILA_H
#define FILA_H

typedef struct fila_ FILA;
typedef struct no_ NO;

FILA* criar_fila();
int entra(FILA* f, int elem);
int sai(FILA* f);
int fila_vazia(FILA* f);

#endif