#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sinopses.h"
#include <unistd.h>

//Struct para guardar os dados de um filme
typedef struct FILME{
	int quantidadePalavras;
	int quantidadeGeneros;
	int ano;
	char nomeFilme[50];
	char generos[3][30];
	char palavras[600][40];
}FILME;

//Funcao auxiliar do MergeSort. Compara duas strings e retorna(strcmp).
int string_compare(const void *A, const void *B) {
      FILME *pA, *pB;
      pA = (FILME *) A;
      pB = (FILME *) B;
      
      return strcmp(pB->nomeFilme, pA->nomeFilme);
}

//MergeSort, utilizado para ordenar o vetor de Filmes(ordena pelo nome).
void MS_sort(void *vector, unsigned long n, size_t memsize, int (*fcmp)(const void *, const void *)) {
	unsigned long middle, rN, j, k;
	void *aux, *r;

	if(n < 2) {
		return;
	}

	middle = (n / 2);
	r = vector + middle * memsize;
	rN = n - middle;
	MS_sort(vector, middle, memsize, fcmp);
	MS_sort(r, rN, memsize, fcmp);

	aux = (void *) malloc(memsize * n);
	j = k = 0;
	while(j + k < n) {
		if(k >= rN || (j < middle && fcmp(vector + memsize * j, r + memsize * k) >= 0)) {
			memcpy(aux + memsize * (j + k), vector + memsize * j, memsize);
			j++;
		} else {
			memcpy(aux + memsize * (j + k), r + memsize * k, memsize);
			k++;
		}
	}
	memcpy(vector, aux, memsize * n);
	free(aux);
}

//Busca binaria no vetor de filmes. Caso encontre, retorna sua posicao no vetor. Caso contrario, retorna -1.
int busca_binaria(FILME *vetor, int l, int r, char *chave) { 
    if (r >= l) { 
        int m = l + (r - l) / 2; 
        if (strcmp(vetor[m].nomeFilme,chave) == 0) return m; 
		if (strcmp(vetor[m].nomeFilme,chave) > 0) return busca_binaria(vetor, l, m - 1, chave); 
        return busca_binaria(vetor, m + 1, r, chave); 
    } 
    return -1; 
} 

//Funcao principal, que calcula a similaridade entre dois filmes. A funcao faz a chamada de um outro programa
//codado em python, o "sinopses.py". Este programa basicamente pega todas as sinopses presentes no "sinopses.txt"
//e faz a "stemizaçao" das palavras na sinopse e depois grava todo o conteudo em um arquivo texto saida, "saida.txt".
//Depois disso, a funcao similaridade vai ler o arquivo saida e fazer o calculo da similaridade a partir das palavras
//stemizadas. 
double similaridade(char *nome1, char *nome2){


	system("python3 sinopses.py"); //Chama o STEMMER em python.

	sleep(2); //Sleep para garantir que antes de inciar a leitura do .txt ele ja tenha sido totalmente processado.

	FILE* arq = fopen("saida.txt", "r");

	int numeroLinhas = 0;      //Total de linhas = Total de "Pessoas" no arquivo csv.
	char caracter;
	while(fscanf(arq, "%c", &caracter) !=EOF){
		if(caracter == '\n')numeroLinhas++; //Cada \n eh uma quebra de linha, ou seja, um registro de dados.
	}

	FILME* vetor = (FILME*) malloc(sizeof(FILME)*10000); //Vetor de filmes.
	int qtd = 1;

	rewind(arq);
	char nome[30];
	int ano;
	char genero[200];

	fscanf(arq, "%*[^:]");
	fscanf(arq, "%*c%*c");
	fscanf(arq, "%[^\n]", nome);
	strcpy(vetor[0].nomeFilme, nome);
	

	fscanf(arq, "%*[^:]");
	fscanf(arq, "%*c%*c");
	fscanf(arq, "%d", &ano);
	
	vetor[0].ano = ano;

	fscanf(arq, "%*[^:]");
	fscanf(arq, "%*c%*c");
	fscanf(arq, "%[^\n]", genero);
	adiciona_generos(genero, vetor, 0);

	

	char teste[30];
	fscanf(arq, "%*[^:]");
	fscanf(arq, "%*c");

	int qtdPalavras = 0;
	
	while(fscanf(arq, "%s", teste) != EOF){
		
		if(strcmp(teste, "nome:") == 0){
			
			vetor[qtd-1].quantidadePalavras = qtdPalavras;
			qtd++;
			qtdPalavras = 0;
			fscanf(arq, "%*c");
			fscanf(arq, "%[^\n]", teste); //Nome
			
			strcpy(vetor[qtd-1].nomeFilme, teste);
			fscanf(arq, "%*[^:]");
			fscanf(arq, "%*c%*c");
			fscanf(arq, "%d", &ano);
			fscanf(arq, "%*c");
			vetor[qtd-1].ano = ano;
			fscanf(arq, "%*[^:]");
			fscanf(arq, "%*c%*c");
			fscanf(arq, "%[^\n]", genero);
			
			adiciona_generos(genero, vetor, qtd-1);
			fscanf(arq, "%*[^:]");
			fscanf(arq, "%*c");

		}
		else{
			
			strcpy(vetor[qtd-1].palavras[qtdPalavras], teste);
			
			qtdPalavras++;
		}
	}
	vetor[qtd-1].quantidadePalavras = qtdPalavras;

	

	MS_sort(vetor, qtd, sizeof(FILME), string_compare);
	


	//C0MECO CALCULO DA SIMILARIDADE

	int filme1 = busca_binaria(vetor,0 , qtd, nome1);
	int filme2 = busca_binaria(vetor,0 , qtd, nome2 );	
	if(filme1 == -1 || filme2 == -1)return -1; //Caso n tenha sinopse, nao e possivel calcular similaridade.
	int generosIguais = 0;
	int totalGeneros = 0;
	double totalPalavras = 0;
	double palavrasIguais = 0;
	double similaridade = 0;
	double aux = 0;

	totalGeneros = vetor[filme1].quantidadeGeneros + vetor[filme2].quantidadeGeneros;
	totalPalavras =(double) vetor[filme1].quantidadePalavras + vetor[filme2].quantidadePalavras;

	for(int i = 0; i<vetor[filme1].quantidadeGeneros; i++){
		for(int j = 0; j<3; j++){
			if(strcmp(vetor[filme1].generos[i], vetor[filme2].generos[j]) == 0){
				generosIguais++;
			}
		}
	}
	
	if(vetor[filme1].quantidadePalavras <= vetor[filme2].quantidadePalavras){
		for(int i = 0; i<vetor[filme1].quantidadePalavras; i++){
			for(int j = 0; j<vetor[filme2].quantidadePalavras; j++){
				if(strcmp(vetor[filme1].palavras[i], vetor[filme2].palavras[j]) == 0){
					palavrasIguais++;
				}
			}
		}
	}
	else{
		for(int i = 0; i<vetor[filme2].quantidadePalavras; i++){
			for(int j = 0; j<vetor[filme1].quantidadePalavras; j++){
				if(strcmp(vetor[filme1].palavras[i], vetor[filme2].palavras[j]) == 0){
					palavrasIguais++;
				}
			}
		}	
	}

	//Calculo Similaridade
	
	if(generosIguais == 1){ 
		similaridade = 0.80;
		aux = (palavrasIguais*3 / totalPalavras) * 0.20;
		if(aux > 0.20) aux = 0.20;
		similaridade += aux;
	}
	else if(generosIguais == 2){
		similaridade = 0.90;
		aux = (palavrasIguais*3 / totalPalavras) * 0.10;
		if(aux > 0.10) aux = 0.10;
		similaridade += aux;
	}
	else if(generosIguais == 3){
		similaridade = 0.95;
		aux = (palavrasIguais*3 / totalPalavras) * 0.05;
		if(aux > 0.05) aux = 0.05;
		similaridade += aux;
	}
	else{
		similaridade = 0.0;
		aux = (palavrasIguais*5 / totalPalavras);
		if(aux > 1.00) aux = 1.00;
		similaridade += aux;
	}


	fclose(arq);
	free(vetor);
	 
	return similaridade;
	

}

//Esta funcao quebra uma string que contem os generos no seguinte formato: "Acao,Aventura,Ficcao Cientifica".
//A funcao quebra no maximo 3 strings, neste caso : "Acao", "Aventura" e "Ficcao Cientifica". Atribui cada uma
//dessas strings a uma posicao em um vetor de generos(que esta na struct FILME).
void adiciona_generos(char *string, FILME* vetor, int pos){

	char *aux = strtok(string, ",");
	int i = 0;
	while(aux != NULL){

		strcpy(vetor[pos].generos[i], aux);
		aux = strtok(NULL, ",");
		i++; 
	}
	vetor[pos].quantidadeGeneros = i;
}


