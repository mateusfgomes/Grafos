#ifndef SINOPSES_H
#define SINOPSES_H
#include <stddef.h>

typedef struct FILME FILME;

int string_compare(const void *A, const void *B);
void MS_sort(void *vector, unsigned long n, size_t memsize, int (*fcmp)(const void *, const void *));
int busca_binaria(FILME *vetor, int l, int r, char *chave);
void adiciona_generos(char *string, FILME* vetor, int pos);
double similaridade(char *nome1, char *nome2);



#endif