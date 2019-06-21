#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grafo.h"
#include "dados.h"
#include "sinopses.h"

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
	char sexo[100];
	char interesse[100];
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
	printf("Sexo: %s\n", atual->sexo);
	printf("Interessado no sexo: %s\n", atual->interesse);	
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
	strcpy(receber->sexo, atual->sexo);
	strcpy(receber->interesse, atual->interesse);
	strcpy(receber->cor, atual->cor);

	
}


int copiaparaAtual(Dados *receber){

	strcpy(atual->usuario, receber->usuario);
	atual->idade = receber->idade;
	strcpy(atual->cidade, receber->cidade);
	strcpy(atual->filme_fav, receber->filme_fav);
	strcpy(atual->time, receber->time);
	strcpy(atual->sexo, receber->sexo);
	strcpy(atual->interesse, receber->interesse);
	strcpy(atual->cor, receber->cor);
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
			else if(strcmp(tag, "sexo") == 0){
				fscanf(arquivo, "%*c");
				fscanf(arquivo, "%[^\n\r]", atual->sexo);
				fscanf(arquivo, "%*c");
			}
			else if(strcmp(tag, "interessado em") == 0){
				fscanf(arquivo, "%*c");
				fscanf(arquivo, "%[^\n\r]", atual->interesse);
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

	return -1;

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

/**
 * Funcao que procura um usuario solicitado para verificar se ele esta' a rede
 */
int procuraSolicitado(Dados **lido, char solicitacao[100]){
	for(int i = 0; i < quantidade_registros; i++){
		if(strcmp(lido[i]->usuario, solicitacao) == 0){
			return i;
		}
	}
	return -1;
}

/**
 * Funcao que verifica se ha solicitacoes recebidas pelo usuario
 */
int *verificaSolicitacoes(Grafo *G, Dados **lido, int usuario, int *quantidade_solicitacoes){

	char arquivo[103];
	int aux, i = 0;
	int *total_solicitacoes = malloc(numero_vertices(G) * sizeof(int));
	FILE *fp;
	memset(arquivo, '\0', 103);

	strcpy(arquivo, lido[usuario]->usuario);
	arquivo[strlen(arquivo)] = '.';
	arquivo[strlen(arquivo)] = 't';
	arquivo[strlen(arquivo)] = 'x';
	arquivo[strlen(arquivo)] = 't';

	fp = fopen(arquivo, "r");

	if(fp == NULL){
		*quantidade_solicitacoes = 0;
		return NULL;		
	}

	while(fscanf(fp, "%d", &aux) != EOF){
		total_solicitacoes[i] = aux;
		i++;
	}

	*quantidade_solicitacoes = i;

	return total_solicitacoes;
}

/**
 * Bubble sort para ordenar na busca por verificacoes se ha' solicitacoes
 * para um determinado usuario
 */
void bubbleSort (int *vetor, int n_elementos) {
    int k, j, aux;
    for (k = 1; k < n_elementos; k++) {
        for (j = 0; j < n_elementos - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                aux  = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = aux;
            }
        }
    }
}

/**
 * Busca Binaria para buscar uma chave, nesse caso, o usuario que esta na
 * lista de solicitacoes ordenada em um vetor
 */
int buscaBinaria(int *vetor, int inicio, int fim, int chave) { 
    if (fim >= inicio) { 
        int meio = inicio + (fim - inicio) / 2; 
  
        if (vetor[meio] == chave) 
            return meio; 
  
        if (vetor[meio] > chave) 
            return buscaBinaria(vetor, inicio, meio - 1, chave); 

        return buscaBinaria(vetor, meio + 1, fim, chave); 
    } 

    return -1; 
} 

/**
 * Funcao que verifica se uma solicitacao de amizade ja foi feita
 * para um determinado usuario, ou se essa pessoa ja eh amiga daquela
 * a qual ela solicita ser
 */
int verificaJaFeitas(Grafo* G, FILE *realizadas, int chave, int solicitado){

	int aux, i = 0;	
	int eh_amigo;

	int *total_solicitacoes = malloc(numero_vertices(G) * sizeof(int));
	
	eh_amigo = procura_amigo(G, chave, solicitado);
	if(eh_amigo){
		return -2;
	}

	while(fscanf(realizadas, "%d", &aux) != EOF){
		total_solicitacoes[i] = aux;
		i++;
	}
	
	if(i > 0){ 
		bubbleSort(total_solicitacoes, i);
		return buscaBinaria(total_solicitacoes, 0 , i, chave);
	}

	return -1;
}

/**
 * Funcao que busca solicitacoes de amizade para um determinado usuario
 * em toda a rede social
 */
void mostraSugestoes(int usuario, Dados **lido, Grafo *G){
	
	double match = 0;
	int flagprint = 0;

	//Busca sugestoes em toda a rede
	for(int i = 0; i < numero_vertices(G); i++){
		
		double filmes = similaridade(lido[i]->filme_fav, lido[usuario]->filme_fav);

		if(i != usuario && strcmp(lido[i]->cidade, lido[usuario]->cidade) == 0){
			match += 0.1;
		}
		if(i != usuario && strcmp(lido[i]->cor, lido[usuario]->cor) == 0){
			match += 0.05;
		}
		if(filmes != -1){
			match += (0.45 * filmes);
			printf("%lf\n\n\n", 0.45 * filmes);
		} else if(i != usuario && strcmp(lido[i]->filme_fav, lido[usuario]->filme_fav) == 0){
			match += 0.45;
		}
		if(i != usuario && abs(lido[i]->idade - lido[usuario]->idade) < 3){
			match += 0.3;
		}
		if(i != usuario && strcmp(lido[i]->time, lido[usuario]->time) == 0){
			match += 0.1;
		}

		if(match > 0.7 && !procura_amigo(G, i, usuario)){
			printf("Pessoa sugerida: %s\n", lido[i]->usuario);
			flagprint = 1;
		}
		match = 0;
	}

	if(flagprint == 0){
		printf("Nao ha' sugestoes para voce no momento\n\n");
		flagprint = 0;
	}

}

/**
 * Funcao para ver a proximidade entre os gostos de determinados usuarios.
 * Se os interesses tiverem 80% de colisoes e as pessoas forem interessadas
 * uma pelo genero da outra, classifica-se um match para namorado(a). Caso
 * contrario, uma porcentagem menor eh dada para amigos
 */
double calculaMatch(Dados **lido, int a, int b){

	double match = 0;

	double filmes = similaridade(lido[a]->filme_fav, lido[b]->filme_fav);

	if(strcmp(lido[a]->cidade, lido[b]->cidade) == 0){
		match += 0.1;
	}
	if(strcmp(lido[a]->cor, lido[b]->cor) == 0){
		match += 0.05;
	}
	if(filmes != -1){
		match += (0.45 * filmes);
	} else if(strcmp(lido[a]->filme_fav, lido[b]->filme_fav) == 0){
		match += 0.45;
	}
	if(abs(lido[a]->idade - lido[b]->idade) < 3){
		match += 0.3;
	}
	if(strcmp(lido[a]->time, lido[b]->time) == 0){
		match += 0.1;
	}
	
	return match;
}

/**
 * Funcao para aceitar uma solicitacao de amizade
 * de determinado usuario (Liga aresta nao direcionada entre
 * os vertices dos dois usuarios no grafo).
 * Informa para o usuario a chance em porcentagem daquela
 * amizade ser verdadeira
 */
int aceitarSolicitacoes(Dados **lido, int *solicitacoes_recebidas, int usuario, Grafo *G){

	int escolha;
	char teste;

	printf("Digite o numero da solicitacao que voce deseja aceitar: ");
	scanf("%d", &escolha);

	double proximidade = calculaMatch(lido, solicitacoes_recebidas[escolha], usuario);

	printf("Chances da amizande escolhida ser verdadeira: %.1lf%%\n", proximidade*100);
	printf("Deseja aceitar?[s/n]\n");
	scanf("%*c");
	scanf("%c", &teste);
	if(teste == 's'){
		inserir_aresta(G, &solicitacoes_recebidas[escolha], &usuario, &proximidade);
	}
	else{
		return -1;
	}

	return escolha;
}

/**
 * Funcao que remove uma solicitacao apos ela ser aceita
 */
void removeSolicitacao(Dados **lido, int *solicitacoes, int quantidade_solicitacoes, int usuario){

	FILE *fp;
	char arquivo[103];
	memset(arquivo, '\0', 103);

	strcpy(arquivo, lido[usuario]->usuario);
	arquivo[strlen(arquivo)] = '.';
	arquivo[strlen(arquivo)] = 't';
	arquivo[strlen(arquivo)] = 'x';
	arquivo[strlen(arquivo)] = 't';

	fp = fopen(arquivo, "w");

	for(int i = 0; i < quantidade_solicitacoes; i++){
		fprintf(fp,"%d\n",solicitacoes[i]);
	}
}

/**
 * Funcao que mostra as possiveis pessoas que nao tem
 * muitos interesses em comum com o usuario logado e as classifica
 * como desnecessarias para esta pessoa
 */
void mostraReducoes(int usuario, Dados **lido, Grafo *G){

	int *reducoes;
	int quantidade_reducoes;

	reducoes = percorre_lista_reducoes(usuario, G, &quantidade_reducoes);

	for(int i = 0; i <quantidade_reducoes; i++){
		printf("Amigo desnecessario: %s\n", lido[reducoes[i]]->usuario);
	}

	free(reducoes);
}

/**
 * Funcao que encontra varias possibilidades de namorado(a) para
 * o usuario logado
 */
void encontraPares(int usuario, Dados **lido, Grafo *G){

	int *pares;
	int quantidade_pares;
	int flagprint = 0;

	pares = percorre_lista_pares(usuario, G, &quantidade_pares);

	for(int i = 0; i <quantidade_pares; i++){
		if(strcmp(lido[pares[i]]->sexo, lido[usuario]->interesse) == 0){
			printf("Possivel Par: %s\n", lido[pares[i]]->usuario);
			flagprint = 1;
		}
	}

	if(!flagprint){
		printf("Nao ha possiveis pares!\n\n");
	}

	free(pares);

}

/**
 * Funcao que lista os amigos de um determinado usuario,
 * isto e, ve todas as arestas de um determinado vertice no grafo
 */
void listarAmigos(Grafo *G, int usuario, Dados **lido){
	
	int *amigos;
	int quantidade_amigos;

	amigos = imprime_adjacencias(G, usuario, &quantidade_amigos);

	for(int i = 0; i < quantidade_amigos; i++){
		printf("%s\n", lido[amigos[i]]->usuario);
	}

	if(quantidade_amigos == 0)
		printf("Voce nao tem amigos!\n\n");

}

/**
 * Funcao que remove um amigo de um determinado usuario, a funcao que mostra
 * as possiveis reducoes nao remove os usuarios, pois isso deve vir da
 * propria vontade do utilizador de remover estas amizades
 * Essa funcao busca para ver se ha de fato o amigo a ser removido, remove a
 * aresta de ambos e em seguida verifica se ha alguma solicitacao de amizade
 * que foi realizada por qualquer uma das partes, apos isso, remove a solicitacao
 */
void removeAmigo(Grafo *G, Dados **lido, int usuario, char solicitacao[100], FILE *realizadas, char arquivo[103]){

	int posicao;
	int erro = 0;
	int i = 0;
	int aux;
	double peso = 0;
	
	posicao = procuraSolicitado(lido, solicitacao);

	if(posicao != -1){
		remover_aresta(G, &usuario, &posicao, &erro, &peso);
		int *total_solicitacoes = malloc(numero_vertices(G) * sizeof(int));

		while(fscanf(realizadas, "%d", &aux) != EOF){
			total_solicitacoes[i] = aux;
			i++;
		}
		fclose(realizadas);
		realizadas = fopen(arquivo, "w+");
		for(int a = 0; a < i; a++){
			if(total_solicitacoes[a] != posicao){
				fprintf(realizadas, "%d\n", total_solicitacoes[a]);
			}
		}
	}
	else{
		printf("Essa pessoa nao eh seu amigo!\n\n");
		return;
	}

	printf("Amigo removido!\n\n");

}

/**
 * Funcao que somente printa o menu e chama as funcoes acima para serem executadas
 * alem de realizar a leitura dos dados
 */
void printaMenu(Grafo *G, Dados **lido, int *usuario){

	int operacao;
	char validar;
	int *solicitacoes_recebidas;
	int quantidade_solicitacoes;
	char arquivo[103];
	char solicitacao[100];
	memset(solicitacao, '\0', 100);
	memset(arquivo, '\0', 103);
	printf("=======================BEM VINDO====================\n");

	printf("Operacoes que podem ser realizadas:\n");
	printf("1 - Fazer solicitacao\n");
	printf("2 - Ver solicitacoes recebidas\n");
	printf("3 - Fazer Logoff\n");
	printf("4 - Mostrar Sugestoes\n");
	printf("5 - Eliminar inimigos\n");
	printf("6 - Encontrar par(es)\n");
	printf("7 - Listar amigos\n");
	printf("8 - Ver perfil\n");
	printf("9 - Remover amigo\n");
	printf("10 - Sair\n");
	scanf("%d", &operacao);

	while(operacao != 10){
		memset(solicitacao, '\0', 100);
		memset(arquivo, '\0', 103);
		switch(operacao){
		case 1:
			printf("Para quem voce deseja enviar a solicitacao: ");
			scanf("%s", solicitacao);
			int verifica = procuraSolicitado(lido, solicitacao);
			if(verifica == -1){
				printf("Este usuario nao existe!\n");
				break;
			}
			strcpy(arquivo, solicitacao);
			arquivo[strlen(arquivo)] = '.';
			arquivo[strlen(arquivo)] = 't';
			arquivo[strlen(arquivo)] = 'x';
			arquivo[strlen(arquivo)] = 't';
			int solicitado = procuraSolicitado(lido, solicitacao);
			FILE *realizadas = fopen(arquivo, "a+");
			if(verificaJaFeitas(G, realizadas, *usuario, solicitado) == -1){
				fprintf(realizadas, "%d\n", *usuario);
				printf("Solicitacao realizada com sucesso!\n\n");
			}
			else if(verificaJaFeitas(G, realizadas, *usuario, solicitado) == -2){
				printf("Esse usuario ja e' seu amigo!\n\n");
			}
			else{
				printf("Solicitacao ja realizada!\n\n");
			}
			fclose(realizadas);
			break;
		case 2:
			solicitacoes_recebidas = verificaSolicitacoes(G, lido, *usuario, &quantidade_solicitacoes);
			printf("==================Solicitacoes===================\n");
			if(solicitacoes_recebidas != NULL && quantidade_solicitacoes != 0) 
				printf("Numero de solicitacoes: %d\n", quantidade_solicitacoes);
			else
				printf("Nao ha solicitacoes\n");
			for(int i = 0; i < quantidade_solicitacoes; i++){
				printf("Solicitação %d: %s\n", i, lido[solicitacoes_recebidas[i]]->usuario);
			}
			if(quantidade_solicitacoes > 0){
				char teste;
				printf("Deseja aceitar alguma solicitacao?[s/n]\n");
				scanf("%*c");
				scanf("%c", &teste);
				if(teste == 's'){
					int escolha = aceitarSolicitacoes(lido, solicitacoes_recebidas, *usuario, G);
					if(escolha == -1){
						printf("Operacao cancelada\n\n");
						free(solicitacoes_recebidas);
						break;
					}
					solicitacoes_recebidas[escolha] = solicitacoes_recebidas[quantidade_solicitacoes - 1];
					quantidade_solicitacoes--;
					removeSolicitacao(lido, solicitacoes_recebidas, quantidade_solicitacoes,*usuario);			
					printf("Solicitacao Aceita!\n\n");		
				}
			}
			free(solicitacoes_recebidas);
			break;
		case 3:
			printf("Logoff realizado!\n\n\n");
			*usuario = printaLogin(lido);
			if(*usuario == -1){
				printf("ERRO NO LOGIN | USUARIO INEXISTENTE\n");
				return;
			}
			break;
		case 4:
			printf("Suas sugestoes de amizades:\n");
			mostraSugestoes(*usuario, lido, G);
			break;
		case 5:
			printf("Mostrando Reducoes\n");
			mostraReducoes(*usuario, lido, G);
			break;
		case 6:
			printf("Encontrando par(es):\n");
			encontraPares(*usuario, lido, G);
			break;
		case 7:
			printf("Sua lista de amigos:\n");
			listarAmigos(G, *usuario, lido);
			break;
		case 8:
			printf("Visualizando o perfil:\n");
			imprimeAtual();
			break;
		case 9:
			printf("Removendo um amigo\n");
			char solicitacao[100];
			memset(solicitacao, '\0', 100);
			printf("Qual amigo voce quer eliminar? ");
			scanf("%*c");
			scanf("%[^\n\r]", solicitacao);
			scanf("%*c");
			verifica = procuraSolicitado(lido, solicitacao);
			if(verifica == -1){
				printf("Este usuario nao existe!\n");
				break;
			}
			strcpy(arquivo, solicitacao);
			arquivo[strlen(arquivo)] = '.';
			arquivo[strlen(arquivo)] = 't';
			arquivo[strlen(arquivo)] = 'x';
			arquivo[strlen(arquivo)] = 't';
			realizadas = fopen(arquivo, "a+");
			removeAmigo(G, lido, *usuario, solicitacao, realizadas, arquivo);
			break;
		default:
			printf("Digite uma opcao valida!\n");
			break;
		}
		printf("Operacoes que podem ser realizadas:\n");
		printf("1 - Fazer solicitacao\n");
		printf("2 - Ver solicitacoes recebidas\n");
		printf("3 - Fazer Logoff\n");
		printf("4 - Mostrar Sugestoes\n");
		printf("5 - Eliminar inimigos\n");
		printf("6 - Encontrar par(es)\n");
		printf("7 - Listar amigos\n");
		printf("8 - Ver perfil\n");
		printf("9 - Remover amigo\n");
		printf("10 - Sair\n");
		scanf("%d", &operacao);
	}
}

/**
 * Funcao para salvar os dados do grafo num arquivo para que, quando
 * fechada a rede social, as pessoas permanecam como amigas umas 
 * das outras
 */
void salvaDados(Grafo *G, FILE *salvar){
	salvaGrafo(G, salvar);
}

/**
 * Primeira funcao a ser executada apos a main, para realizar o
 * carregamento dos dados da rede social
 */
void carregaDados(FILE *salvo, Grafo *G, int n_registros, Dados **lido){

	char teste, teste2;
	int usuario;
	int amigos[n_registros];
	double afinidade[n_registros];
	for(int h = 0; h < n_registros; h++){
		amigos[h] = 0;
		afinidade[h] = 0;
	}
	int i = 0;

	while(fscanf(salvo, "%c\n", &teste) != EOF){
		if(teste == '-'){
			fscanf(salvo, "%d\n", &usuario);
		}
		else{
			while(teste2 != '-'){
				fseek(salvo, -1, SEEK_CUR);
				fscanf(salvo, "%d,%lf\n", &amigos[i], &afinidade[i]);
				fscanf(salvo, "%c", &teste2);
				i++;
			}
			fseek(salvo, -1, SEEK_CUR);
			teste2 = '\0';
		}
		for(int j = 0; j < i; j++){
			inserir_aresta_direcionado(G, &usuario, &amigos[j], &afinidade[j]);
		}
		i = 0;
	}

}

/**
 * Funcao para realizacao do cadastro de uma determinada pessoa que deseja 
 * entrar para a rede social, assim, os cadastros nao precisam mais
 * ser adicionados diretamente no arquivo de texto
 */
void escreveArquivoAtual(FILE *arquivo){

	fseek(arquivo, 0, SEEK_END);
	fprintf(arquivo, "\n");
	fprintf(arquivo, "nome do usuario: ");
	fprintf(arquivo, "%s\n", atual->usuario);
	fprintf(arquivo, "idade: ");
	fprintf(arquivo, "%d\n", atual->idade);
	fprintf(arquivo, "cidade: ");
	fprintf(arquivo, "%s\n", atual->cidade);
	fprintf(arquivo, "filme predileto: ");
	fprintf(arquivo, "%s\n", atual->filme_fav);
	fprintf(arquivo, "time: ");
	fprintf(arquivo, "%s\n", atual->time);
	fprintf(arquivo, "sexo: ");
	fprintf(arquivo, "%s\n", atual->sexo);
	fprintf(arquivo, "interessado em: ");
	fprintf(arquivo, "%s\n", atual->interesse);
	fprintf(arquivo, "cor predileta: ");
	fprintf(arquivo, "%s\n", atual->cor);
}

/**
 * Escreve o arquivo que relaciona as senhas com os
 * nomes de usuario, no arquivo "logins.csv", para aqueles
 * usuarios os quais desejam se cadastrar agora
 */
void escreveArquivoSenha(){
	FILE *escreve = fopen("logins.csv", "r+");
	fseek(escreve, 0, SEEK_END);
	fprintf(escreve, "%s,", atual->usuario);
	fprintf(escreve, "%s\n", atual->senha);
	fclose(escreve);
}

/**
 * Funcao para realizar a coleta de dados
 * pelo terminal do nomo usuario que deseja se cadastrar
 */
void criaConta(FILE *arquivo){

	int sexo;
	int interesse;

	printf("Digite seu nome: ");
 	scanf("%[^\n\r]", atual->usuario);
	scanf("%*c");
	printf("Digite sua senha: ");	
 	scanf("%[^\n\r]", atual->senha);
	scanf("%*c");
	printf("Digite sua cidade atual: ");
 	scanf("%[^\n\r]", atual->cidade);
	scanf("%*c");
	printf("Digite sua cor favorita: ");
 	scanf("%[^\n\r]", atual->cor);
	scanf("%*c");
	printf("Digite seu filme favorito: ");
 	scanf("%[^\n\r]", atual->filme_fav);
	scanf("%*c");
	printf("Digite seu time: ");
	scanf("%[^\n\r]", atual->time);
	scanf("%*c");
	printf("Digite sua idade: ");
 	scanf("%d", &(atual->idade));
	printf("Qual seu sexo?\n[1] Masculino   [2] Feminino\n");	
 	scanf("%d", &sexo);
	if(sexo == 1){
		strcpy(atual->sexo, "masculino");
	}
	else if(sexo == 2){
		strcpy(atual->sexo, "feminino");
	}
	printf("Voce e' interessado no sexo...?\n[1] Masculino   [2] Feminino\n");	
	scanf("%d", &interesse);
	if(interesse == 1){
		strcpy(atual->interesse, "masculino");
	}
	else if(interesse == 2){
		strcpy(atual->interesse, "feminino");
	}

	escreveArquivoSenha();
	escreveArquivoAtual(arquivo);

}

int main(void){

	int usuario_logado; //controla qual o usuario atual
	char opcao; //controla se ha necessidade de criar uma nova conta

	FILE *arquivo = fopen("dados.txt", "r+"); //arquivo de cadastrados
	FILE *carregar = fopen("salvo.txt", "r"); //arquivo que guarda o grafo antigo

	atual = (Dados*) malloc(sizeof(Dados));

	//Vetor que controla os lidos de um arquivo txt
	Dados **lido = (Dados**) malloc(100*sizeof(Dados*));
	for(int i = 0; i < 100; i++){
		lido[i] = (Dados*) malloc(sizeof(Dados));
	}

	printf("Deseja criar uma conta?[s/n]\n");
	scanf("%c", &opcao);
	scanf("%*c");
	if(opcao == 's') criaConta(arquivo); 
	rewind(arquivo);
	leArquivo(arquivo, lido);
	usuario_logado = printaLogin(lido);
	Grafo *G = criar_grafo(&quantidade_registros);
	
	if(carregar != NULL){ //Carrega os dados do grafo que existia anteriormente, se existia
		carregaDados(carregar, G, quantidade_registros, lido);
		fclose(carregar);
	}
	
	if(usuario_logado != -1){
		copiaparaAtual(lido[usuario_logado]);
		printaMenu(G, lido, &usuario_logado);
	}
	else{
		printf("ERRO NO LOGIN | USUARIO INVALIDO\n\n");
	}

	FILE *salvar = fopen("salvo.txt", "w");

	salvaDados(G, salvar);

	for(int i = 0; i < 100; i++){
		free(lido[i]);
	}
	free(lido);
	free(atual);
	fclose(arquivo);
	return 0;
}