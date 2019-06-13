#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "dados.h"

/**
 * Estrutura de um usuario
**/
typedef struct Dados_{
    char usuario[100];
    char cidade[100];
    char filme_fav[100];
    char time[100];
    char cor[100];
    char senha[100];
    int idade;
}Dados;

Dados* atual; //Variavel Global para guardar o usuario atual (foi usada como auxiliar para ler o arquivo).
			  //Mas creio que possa ser util mais para frente para definir qual o usuario logado
int quantidade_registros; //Numero de usuarios que aquele arquivo contem

/**
 * Funcao que imprime os dados do usuario atual
**/
void imprimeAtual(){

	printf("Nome de usuario: %s\n", atual->usuario);
	printf("Idade: %d\n", atual->idade);
	printf("Cidade: %s\n", atual->cidade);
	printf("Filme Favorito: %s\n", atual->filme_fav);
	printf("Time: %s\n", atual->time);
	printf("Cor Favorita: %s\n", atual->cor);
	printf("\n");

}

/**
 * Funcao que imprime o vetor de dados lido do arquivo, cada usuario do arquivo e' um item do vetor
**/
void imprimeDados(Dados **lido, int tam){

	for(int h = 0; h < tam; h++){
		printf("Nome de usuario: %s\n", lido[h]->usuario);
		printf("Idade: %d\n", lido[h]->idade);
		printf("Cidade: %s\n", lido[h]->cidade);
		printf("Filme Favorito: %s\n", lido[h]->filme_fav);
		printf("Time: %s\n", lido[h]->time);
		printf("Cor Favorita: %s\n", lido[h]->cor);
		printf("\n");
	}

}

/**
 * Para copiar o usuario (atual (aqui usado como auxiliar)) para um item do vetor.
 * Nao utilizei '=' para assimilar os valores, pois quando o proximo valor fosse lido do arquivo,
 * iria ser carregado para o mesmo endereco, o que ia causar um problema no vetor, portanto,
 * a melhor saida foi copiar dado a dado
**/  
int copiaAtual(Dados *receber){

	strcpy(receber->usuario, atual->usuario);
	receber->idade = atual->idade;
	strcpy(receber->cidade, atual->cidade);
	strcpy(receber->filme_fav, atual->filme_fav);
	strcpy(receber->time, atual->time);
	strcpy(receber->cor, atual->cor);

}

/**
 * Funcao que le o arquivo de dados fornecido como entrada exatamente como o mostrado no pdf
**/
int leArquivo(FILE* arquivo, Dados **lido){
	int i = 0, tam = 0;
	char teste;
	char tag[100];
	memset(tag, '\0', 100); //Colocando '\0' no vetor de tag. Tags sao os nomes dos campos de cada usuario

	while(fscanf(arquivo, "%c", &teste) != EOF){ //Le CHAR por CHAR
		if(teste == ':'){ //Achou ':', guarda a tag e compara pra ver qual vai ser o valor do respectivo campo
			if(strcmp(tag, "nome do usuario") == 0){ //se for nome
				fscanf(arquivo, "%*c"); //ignorando a ','
				fscanf(arquivo, "%[^\n\r]", atual->usuario);
				fscanf(arquivo, "%*c"); //ignorando o '\n'
			}
			else if(strcmp(tag, "idade") == 0){ //idade
				fscanf(arquivo, "%*c"); //ignorando a ','
				fscanf(arquivo, "%d", &(atual->idade));
				fscanf(arquivo, "%*c"); //ignorando o '\n'
			}
			else if(strcmp(tag, "cidade") == 0){ //cidade
				fscanf(arquivo, "%*c"); //ignorando a ','
				fscanf(arquivo, "%[^\n\r]", atual->cidade);
				fscanf(arquivo, "%*c"); //ignorando o '\n'
			} 
			else if(strcmp(tag, "filme predileto") == 0){
				fscanf(arquivo, "%*c"); //idem anteriores daqui para baixo
				fscanf(arquivo, "%[^\n\r]", atual->filme_fav);
				fscanf(arquivo, "%*c");
			}
			else if(strcmp(tag, "time") == 0){
				fscanf(arquivo, "%*c");
				fscanf(arquivo, "%[^\n\r]", atual->time);
				fscanf(arquivo, "%*c");
			}
			else if(strcmp(tag, "cor predileta") == 0){
				fscanf(arquivo, "%*c");
				fscanf(arquivo, "%[^\n\r]", atual->cor);
				fscanf(arquivo, "%*c%*c");
				copiaAtual(lido[tam]);
				tam++;
			}
			i = 0;
			memset(tag, '\0', 100); //zerando a tag novamente
		}
		else{
			tag[i] = teste; //se nao for ':' significa que ta guardando a tag ainda
			i++;
		}
	}

	quantidade_registros = tam; //guardando a quantidade de usuarios que este arquivo contem
}

/**
 * Funcao que verifica num arquivo csv se o usuario existe e digitou sua senha certo
 * Parametros: login[100]: o login digitado na entrada do terminal
 * 			   senha[100]: senha digitada na entrada do terminal
 * 			   fp: arquivo de senhas e logins
 * Retorna 1 se existe e acertou sim e 0, caso contrario
**/
int verificaLogin(char login[100], char senha[100], FILE* fp){

	char login_correto[100]; //Login para percorrer o arquivo verificando
	char senha_correto[100]; //Senha para percorrer o arquivo verificando
	char ignora[100]; //Para ignorar caso nao seja o nome do login desejado
	int posicao = 0;

	memset(login_correto, '\0', 100); //zera os campos
	memset(senha_correto, '\0', 100);
	memset(ignora, '\0', 100);

	while(fscanf(fp, "%[^,]", login_correto) != EOF){ //Enquanto nao estiver no fim do arquivo
		fscanf(fp, "%*c");							  //Le ate a virgula e ignora ela em seguida
		if(strcmp(login, login_correto) == 0){ //Se achou o login
			fscanf(fp, "%[^\r\n]", senha_correto); //Ve se a senha esta certa
			if(strcmp(senha, senha_correto) == 0){ //Se sim, loga
				printf("Voce logou\n");
				return posicao;
			}
			else{
				printf("Senha incorreta, digite novamente\n"); //Se nao, deixa tentar somente mais uma vez
				scanf("%s", senha);
				if(strcmp(senha, senha_correto) == 0){
					printf("Voce logou\n");
					return posicao;
				}
				else{
					printf("Voce digitou a senha incorreta mais de uma vez, encerrando...\n");
					return -1;
				}
			}
		}
		else{
			fscanf(fp, "%[^\r\n]", ignora); //Se o login nao e o desejado precisamos ignorar a senha que vem a frente
			fscanf(fp, "%*c"); //Ignora o '\n'
			posicao++;
		}
		memset(login_correto, '\0', 100); //Reseta os valores
		memset(senha_correto, '\0', 10);
		memset(ignora, '\0', 100);
	}

	//TODO: NOME DE USUARIO INVALIDO, CHAMA A RECURSAO

}

/**
 * Funcao para printar a entrada do programa
 * com a tela de login e senha
**/
int printaLogin(Dados **lido){

	char login[100];
	char senha[10];
	int usuario_logado;

	FILE *verifica = fopen("logins.csv", "r");
	memset(login, '\0', 100);
	memset(senha, '\0', 10);

	printf("=========BEM VINDO==========\n");
	printf("Digite seu nome de usuario: ");
	scanf("%s", login);
	printf("Digite sua senha: ");
	scanf("%s", senha);
	usuario_logado = verificaLogin(login, senha, verifica);
	fclose(verifica);

	return usuario_logado;

}

int procuraSolicitado(Dados **lido, char solicitacao[100]){
	for(int i = 0; i < quantidade_registros; i++){
		if(strcmp(lido[i]->usuario, solicitacao) == 0){
			return i;
		}
	}
	return -1;
}

void verificaSolicitacoes(Grafo *G, int usuario){



}

void printaMenu(Grafo *G, Dados **lido, int usuario){

	int operacao;
	int tipo;
	char arquivo[103];
	char solicitacao[100];
	memset(solicitacao, '\0', 100);
	memset(arquivo, '\0', 103);
	strcpy(arquivo, lido[usuario]->usuario);
	arquivo[strlen(arquivo)] = '.';
	arquivo[strlen(arquivo)] = 't';
	arquivo[strlen(arquivo)] = 'x';
	arquivo[strlen(arquivo)] = 't';

	scanf("%d", &operacao);

	switch (operacao){
	case 1:
		scanf("%s", solicitacao);
		int solicitado = procuraSolicitado(lido, solicitacao);
		tipo = 0;
		FILE *realizadas = fopen(arquivo, "a+");
		fprintf(realizadas, "%d\n", solicitado);
		imprime_grafo(G);
		break;
	case 2:
		verificaSolicitacoes(G, usuario);
		break;
	}
}

int main(void){

	int usuario_logado;

	FILE *arquivo = fopen("dados.txt", "r");
	atual = (Dados*) malloc(sizeof(Dados));

	Dados **lido = (Dados**) malloc(100*sizeof(Dados*));
	for(int i = 0; i < 100; i++){
		lido[i] = (Dados*) malloc(sizeof(Dados));
	}
	leArquivo(arquivo, lido);
	usuario_logado = printaLogin(lido);
	Grafo *G = criar_grafo(&quantidade_registros);
	imprime_grafo(G);

	if(usuario_logado != -1){
		printaMenu(G, lido, usuario_logado);
	}

	for(int i = 0; i < 100; i++){
		free(lido[i]);
	}
	free(lido);
	free(atual);
	fclose(arquivo);
	return 0;
}