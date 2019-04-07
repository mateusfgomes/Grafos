#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grafo.h"
/*
Programa que detecta a similaridade entre dois textos (.txt) utilizando grafos

Mateus Ferreira Gomes
---------
Universidade de São Paulo
Instituto de Ciências Matemáticas e de Computação
*/
int verifica(FILE* fp){

  FILE *save;
  save = fp;
  char aux;

  while(1){
    fscanf(fp, "%c", &aux);
    if(aux == '/'){
      fp = save;
      return 1;
    }
    else if(aux == '#'){
      fp = save;
      return 0;
    }
  }


}

void ler_arquivo(FILE* fp, char palavras[100]){

  int i = 0;
  int j = 0;
  char aux;
  strcpy(palavras,"");


  while(fscanf(fp, "%c", &aux) != EOF){
      palavras[j] = aux;
      j++;
  }

}

void remover_repetidas(char minusculas[10000][45], int tam_min, char maiusculas[10000][45], int* tam_maius, char** palavras, int n_palavras){

    int count_removidas = 0;

    for(int i = 0; i < tam_min; i++){
        for(int j = 0; j < (*tam_maius); j++){
            if(strcmp(minusculas[i], maiusculas[j]) == 0){
                memset(maiusculas[j], '\0', 45);
                count_removidas++;
            }
        }
    }

    for(int i = 0; i < (*tam_maius)-1; i++){
        if(maiusculas[i][0] == '\0'){
            for(int j = i; j < (*tam_maius); j++){
                if(maiusculas[j][0] != '\0'){
                    strcpy(maiusculas[i], maiusculas[j]);
                    memset(maiusculas[j], '\0', 45);
                    break;
                }
            }
        }
    }

    *tam_maius -= count_removidas;


}

char** verbo_subst(char minusculas[10000][45], int tam_min, char maiusculas[10000][45], int tam_maius, int* classes_palavras, int* n_verbos, int* n_substantivos, int* n_classe, char** classes){

	char** palavras_uteis;
	int n_palavras = 0;


    palavras_uteis = (char**) malloc(20000*sizeof(char*));
    for(int i = 0; i < 20000; i++){
        palavras_uteis[i] = (char*) malloc(45*sizeof(char));
    }

    for(int x = 0; x < *n_classe; x++){
        classes_palavras[x] = -1;
    }

	for(int x = 0; x < 20000; x++){
		for(int r = 0; r < 45; r++){
			palavras_uteis[x][r] = '\0';
		}
	}

    remover_repetidas(minusculas, tam_min, maiusculas, &tam_maius, palavras_uteis, n_palavras);

	for(int x = 0; x < *n_classe; x++){
		if(strcmp(classes[x], "CN") == 0){

			strcpy(palavras_uteis[n_palavras], minusculas[x]);
            classes_palavras[n_palavras] = 1;
            n_palavras++;
        }
        else if(classes[x][0] == 'V'){

            strcpy(palavras_uteis[n_palavras], maiusculas[(*n_verbos)]);
            classes_palavras[n_palavras] = 0;
            n_palavras++;
            (*n_verbos)++;
        }
    }
    (*n_substantivos) = n_palavras - (*n_verbos);

    return palavras_uteis;
}

char** verifica_classes(char** palavras, int* n_palavras, int* classes_palavras, int* n_substantivos, int* n_verbos, int* n_classe, char** classes){

	int tampalavra;
	int pos1 = 0;
	int pos2 = 0;
	char maiusculas[10000][45];
	char minusculas[10000][45];

	for(int x = 0; x < 10000; x++){
		for(int r = 0; r < 45; r++){
			minusculas[x][r] = '\0';
		}
	}
	for(int x = 0; x < 10000; x++){
		for(int r = 0; r < 45; r++){
			maiusculas[x][r] = '\0';
		}
	}


	for(int x = 0; x < *n_palavras; x++){
		if(palavras[x][0] >= 65 && palavras[x][0] <= 90){
			strcpy(maiusculas[pos1], palavras[x]);
			pos1++;
		}
		else if(palavras[x][0] >= 97 && palavras[x][0] <= 122){
			strcpy(minusculas[pos2], palavras[x]);
			pos2++;
		}
	}


	for(int x = 0; x < *n_palavras; x++){
		tampalavra = strlen(maiusculas[x]);
		for(int r = 0; r < tampalavra; r++){
			if(maiusculas[x][r] >= 65 && maiusculas[x][r] <= 90){
				maiusculas[x][r] += 32;
			}
		}
	}

	return verbo_subst(minusculas, pos2, maiusculas, pos1, classes_palavras, n_verbos, n_substantivos, n_classe, classes);



}

void classe(char aux[45], int* size, int* n_classe, char** classes){


  for(int h = (*size)-1; h >= 0; h--){
    if(aux[h] >= 65 && aux[h] <= 90){
      while(h >= 0){
        classes[(*n_classe)][h] = aux[h];
        h--;
      }
      (*n_classe)++;
    }
  }


}

void le_ate_barra(char* palavras, char aux[45], char** ans, int* i, int* j, int* n_classe, char** classes){

  int g = 0;
  int flag = 1;

  while(palavras[*i] != '/' && palavras[*i] != '<'){
    if(palavras[*i] != ' '){
      aux[g] = palavras[*i];
      g++;
    }
    else{
      classe(aux, &g, n_classe, classes);
      for(int p = 0; p < 45; p++){
        aux[p] = '\0';
      }
      g = 0;
      flag = 0;
    }
    (*i)++;
  }
  g = 0;
  if(flag) strcpy(ans[*j], aux);
  if(flag){
    (*j)++;
  }
  flag = 1;
  for(int p = 0; p < 45; p++){
    aux[p] = '\0';
  }
}

char** separar_string(char* palavras, int* n_palavras, int* n_classe, char** classes){

  int i = 6;
  int j = *n_palavras;
  int k = 0;
  int tipos[100];
  char aux[45];
  int flag2 = 0;
  int flag_barra = 0;
  char** classes_palavras;

  char** ans = (char**) malloc(100000*sizeof(char*));
  for(int b = 0; b < 100000; b++){
    ans[b] = (char*) malloc(45*sizeof(char));
  }

  for(int y = 0; y < 100000; y++){
    for(int p = 0; p < 45; p++){
      ans[y][p] = '\0';
    }
  }

  for(int p = 0; p < 45; p++){
    aux[p] = '\0';
  }

  while(palavras[i] != '<'){
    if(palavras[i] == ' '){
      i++;
      while((palavras[i] >= 97 && palavras[i] <= 122) && palavras[i] != '/'){
        ans[j][k] = palavras[i];
        i++;
        k++;
        flag2 = 1;
      }
      k = 0;
      if(flag2){
        j++;
        flag2 = 0;
      }
      i++;
    }
    i++;
  }
  i = 6;

  while(palavras[i] != '/' || palavras[i+1] != 's'){
    if(palavras[i] == '/' && flag_barra == 0){
      i++;
      le_ate_barra(palavras, aux, ans, &i, &j, n_classe, classes);
      flag_barra = 1;
    }
    else if(flag_barra == 1){
      le_ate_barra(palavras, aux, ans, &i, &j, n_classe, classes);
    }
    i++;
  }

  *n_palavras = j;
  return ans;

}

int main(void){


  char* dados;
  char* dados2;
  FILE* arquivo1;
  FILE* arquivo2;
  char** palavras;
  char** palavras2;
  char** palavras_uteis;
  char** palavras_uteis2;
  int n_palavras = 0;
  int n_palavras2 = 0;
  Grafo* G;
  Grafo* G2;
  int erro = 0;
  int substantivos[10000];
  int substantivos2[10000];
  int n_palavras_uteis;
  int n_palavras_uteis2;
  int* classes_palavras;
  int* classes_palavras2;
  int achou_verbo = 0;
  int peso = 0;
  int n_verbos = 0;
  int n_substantivos = 0;
  int n_verbos2 = 0;
  int n_substantivos2 = 0;
  char** classes;
  int n_classe = 0;
  char** classes2;
  int n_classe2 = 0;
  double resposta = 0;
  char nome1[40];
  char nome2[40];

  scanf("%s", nome1);
  scanf("%s", nome2);

  classes = (char**) malloc(10000*sizeof(char*));
  for(int i = 0; i < 10000; i++)
        classes[i] = (char*) malloc(10*sizeof(char));

  classes2 = (char**) malloc(10000*sizeof(char*));
  for(int i = 0; i < 10000; i++)
        classes2[i] = (char*) malloc(10*sizeof(char));

  classes_palavras = (int*) malloc(10000*sizeof(int));
  classes_palavras2 = (int*) malloc(10000*sizeof(int));

  for(int i = 0; i < 10000; i++){
      classes_palavras[i] = -1;
  }

  for(int i = 0; i < 10000; i++){
      substantivos2[i] = -1;
  }

  for(int i = 0; i < 10000; i++){
    for (int j = 0; j < 10; j++) {
      classes[i][j] = '\0';
    }
  }


  dados = (char*) malloc(100000*sizeof(char));
  dados2 = (char*) malloc(100000*sizeof(char));

  arquivo1 = fopen(nome1, "r");


  ler_arquivo(arquivo1, dados);

  palavras = separar_string(dados, &n_palavras, &n_classe, classes);

  palavras_uteis = verifica_classes(palavras, &n_palavras, classes_palavras, &n_substantivos, &n_verbos, &n_classe, classes);



  G = criar_grafo(&n_substantivos, &erro);

  int contador = 0;

  int j = 0;



  n_palavras_uteis = n_substantivos + n_verbos;



  for(int i = 0; i < n_palavras_uteis; i++){

      if(classes_palavras[i] == 1 && achou_verbo == 1 && j > 1){
          inserir_aresta(G, &substantivos[j-1], &contador, &peso, &erro);
          substantivos[j-1] = -1;
          j--;
          achou_verbo = 0;
      }
      else if(classes_palavras[i]){
          substantivos[j] = contador;
          contador++;
          j++;
      }
      else if(classes_palavras[i] == 0){
          achou_verbo = 1;
      }

  }


  contador = 0;
  j = 0;
  achou_verbo = 0;

  arquivo2 = fopen(nome2, "r");

  ler_arquivo(arquivo2, dados2);

  palavras2 = separar_string(dados2, &n_palavras2, &n_classe2, classes2);


  palavras_uteis2 = verifica_classes(palavras2, &n_palavras2, classes_palavras2, &n_substantivos2, &n_verbos2, &n_classe2, classes2);


  n_palavras_uteis2 = n_substantivos2 + n_verbos2;



  G2 = criar_grafo(&n_substantivos2, &erro);


  for(int i = 0; i < n_palavras_uteis2; i++){

      if(classes_palavras2[i] == 1 && achou_verbo == 1 && j > 1){
          inserir_aresta(G2, &substantivos2[j-1], &contador, &peso, &erro);
          substantivos2[j-1] = -1;
          j--;
          achou_verbo = 0;
      }
      else if(classes_palavras2[i]){
          substantivos2[j] = contador;
          contador++;
          j++;
      }
      else if(classes_palavras2[i] == 0){
          achou_verbo = 1;
      }

  }
  double receptor;

  receptor = compara_grafos(G, G2, n_verbos, n_verbos2);



  resposta += receptor*25;
  double count = 0;

  int batem = 0;
  double batem2 = 0;

  if(n_palavras_uteis > n_palavras_uteis2){
     batem = n_palavras_uteis2;
  }
  else {
     batem = n_palavras_uteis;
  }

  if(n_palavras_uteis < n_palavras_uteis2){
     batem2 = n_palavras_uteis2;
  }
  else {
     batem2 = n_palavras_uteis;
  }

  for(int i = 0; i < batem; i++){
     if(classes_palavras[i] == classes_palavras2[i]) count++;
  }

  count /= batem;


  count *= 35;


  resposta += count;


  double count2 = 0;



  for(int i = 0; i < batem; i++){
     if(strcmp(palavras_uteis[i], palavras_uteis2[i]) == 0) count2++;
  }

  count2 /= batem2;


  count2 *= 40;

  resposta += count2;


  printf("%.2lf%%\n", resposta);

  free(dados2);
  free(dados);
  free(classes_palavras);
  free(classes_palavras2);
  free(palavras);
  free(palavras_uteis);
  free(palavras2);
  free(palavras_uteis2);

  return 0;
}
