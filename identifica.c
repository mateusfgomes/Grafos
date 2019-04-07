#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char classes[10000][10];
int n_classe = 0;

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

void classe(char aux[45], int* size){


  for(int h = (*size)-1; h >= 0; h--){
    if(aux[h] >= 65 && aux[h] <= 90){
      while(h >= 0){
        classes[n_classe][h] = aux[h];
        h--;
      }
      n_classe++;
    }
  }

  verifica_classes(); // TODO

}

void le_ate_barra(char* palavras, char aux[45], char** ans, int* i, int* j){

  int g = 0;
  int flag = 1;

  while(palavras[*i] != '/' && palavras[*i] != '<'){
    //printf("%s\n", aux);
    if(palavras[*i] != ' '){
      aux[g] = palavras[*i];
      g++;
    }
    else{
      classe(aux, &g);
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

void separar_string(char* palavras){

  int i = 6;
  int j = 0;
  int k = 0;
  int tipos[100];
  char aux[45];
  int flag2 = 0;
  int flag_barra = 0;

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


  //for(int i = 0; i < 4; i++)
  //printf("%d\n", j);

  i = 6;

  while(palavras[i] != '/' || palavras[i+1] != 's'){
    //printf("%c\n", palavras[i]);
    if(palavras[i] == '/' && flag_barra == 0){
      i++;
      le_ate_barra(palavras, aux, ans, &i, &j);
      flag_barra = 1;
    }
    else if(flag_barra == 1){
      le_ate_barra(palavras, aux, ans, &i, &j);
    }
    i++;
  }

  for(int l = 0; l < j; l++)
    printf("%s\n", ans[l]);

  for(int l = 0; l < n_classe; l++)
    printf("%s\n", classes[l]);


}

int main(void){

  char* palavras;
  FILE* arquivo1;

  for(int i = 0; i < 10000; i++){
    for (int j = 0; j < 10; j++) {
      classes[i][j] = '\0';
    }
  }


  palavras = (char*) malloc(100000*sizeof(char));

  arquivo1 = fopen("5.txt", "r");

  ler_arquivo(arquivo1, palavras);

  //printf("%s\n", palavras);

  separar_string(palavras);

  //printf("\n");

  free(palavras);

  return 0;
}
