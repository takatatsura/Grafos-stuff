#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define MAX 20

typedef struct no_lista_adj{
	int vertice;
	int peso;
	struct no_lista_adj *prox;
} no_lista_adj_T, *no_lista_adj_P;

typedef struct lista_adj{
	int n_vizinhos;
	no_lista_adj_T *cabeca;
} lista_adj_T, *lista_adj_P;

typedef struct grafos{
	int tipo_grafo; 			//0- não direcionado	1- não direcionado com peso		2- direcionado		3- direcionado com peso
	int n_vertices;
	lista_adj_P vetorListas;		//vetor com cada lista adjacente de cada vertice
} grafo_T, *grafo_P;



no_lista_adj_P cria_no (int v) {
	no_lista_adj_P no = (no_lista_adj_P) malloc (sizeof(struct no_lista_adj));
	if (!no){
		printf("Erro fatal: nao foi possivel criar o no.\nSaindo...\n");
    	exit(1);
	}
	
	no->vertice = v;
	no->prox = NULL;
	no->peso = 1;
	
	return no;
}

grafo_P cria_grafo (int n, int tipo) {
	int i;
	grafo_P grafo = (grafo_P) malloc (sizeof(struct grafos));
	if (!grafo){
		printf("Erro fatal: nao foi possivel criar o grafo. \nSaindo...\n");
    	exit(1);
	}
	
	grafo->n_vertices = n;
	grafo->tipo_grafo = tipo;
	
	//Cria as listas de adjacencias
	grafo->vetorListas = (lista_adj_P) malloc (n * sizeof(struct lista_adj));
	if (!(grafo->vetorListas)){
		printf("Erro fatal: nao foi possivel criar a lista de adjacencia. \nSaindo...\n");
    	exit(1);
	}
	
	for (i = 0; i < n; i++){
		grafo->vetorListas[i].cabeca = NULL;
		grafo->vetorListas[i].n_vizinhos = 0;
	}
	
	return grafo;
}

void mataGrafo (grafo_P grafo){
	if (grafo){
		if(grafo->vetorListas){
			int i;
			
			for (i = 0; i < grafo->n_vertices; i++){
				no_lista_adj_P lista = grafo->vetorListas[i].cabeca;
				
				while (lista) {
					no_lista_adj_P aux = lista;
					lista = lista->prox;
					free(aux);
				}
				
			}
			
			free(grafo->vetorListas);
		}
		
		free(grafo);
	}
	
}

void insere (grafo_T *grafo, int origem, int destino, int peso){
	no_lista_adj_P novoNo = cria_no(destino);
	
	if (grafo->tipo_grafo == 1 || grafo->tipo_grafo == 3)
		novoNo->peso = peso;
			
	novoNo->prox = grafo->vetorListas[origem].cabeca;
	grafo->vetorListas[origem].cabeca = novoNo;
	grafo->vetorListas[origem].n_vizinhos++;
	
	
	if (grafo->tipo_grafo == 0 || grafo->tipo_grafo == 1) {  //nao direcionado
		novoNo = cria_no(origem);
		
		if (grafo->tipo_grafo == 1)
			novoNo->peso = peso;
			
		novoNo->prox = grafo->vetorListas[destino].cabeca;
		grafo->vetorListas[destino].cabeca = novoNo;
		grafo->vetorListas[destino].n_vizinhos++;
	}
}

void imprime (grafo_P grafo){
	int i;
	for (i = 0; i < grafo->n_vertices; i++){
		no_lista_adj_P lista = grafo->vetorListas[i].cabeca;
		
		printf("\n%d: ", i);
        while (lista) {
        	if (grafo->tipo_grafo == 1 || grafo->tipo_grafo == 3)
            	printf("[%d| %d]->", lista->vertice, lista->peso);
            else
            	printf("[%d]->", lista->vertice);
            lista = lista->prox;
        }
        printf("NULL");
	}
	
}

//--------------------------------------------------------------

void busca_profundidade (grafo_T *grafo, int v, int visitado[]){
	no_lista_adj_P no = grafo->vetorListas[v].cabeca;
	int i;
	
	printf("\nVertice visitado: %d", v);
	/*if (visitado[no->vertice])
		return;*/
	visitado[v] = 1;
	
	while (no != NULL) {
		if (!visitado[no->vertice])
			busca_profundidade(grafo, no->vertice, visitado);
		no = no->prox;
	}
	return;
}

void busca_profundidade_init (grafo_T *grafo, int inicio){
	int i, visitado[MAX];
	
	no_lista_adj_P no = grafo->vetorListas[inicio].cabeca;
	for (i = 0; i < MAX; i++)
		visitado[i] = 0;
	
		
	busca_profundidade(grafo, inicio, visitado);
	return;
}

//----------------------------------------------------------------

int minDist (int dist[], int visitado[], int n){
	int min = 1000000, indice_min = -1, i;
	
	for (i = 0; i < n; i++){
		if (!visitado[i] && dist[i] <= min){
			min = dist[i];
			indice_min = i;
		}
	}
	
	return indice_min;
}

void Djikstra (grafo_T *grafo, int origem, int dist[], int visitado[]){
	no_lista_adj_P no = grafo->vetorListas[origem].cabeca;
	int prox;
	
	visitado[origem] = 1;
	
	while (no != NULL) {
		if (dist[origem] + no->peso < dist[no->vertice] /*&& !visitado[no->vertice]*/){
			dist[no->vertice] = dist[origem] + no->peso;
		}
		no = no->prox;
	}
	
	prox = minDist(dist, visitado, grafo->n_vertices); 			//qual o próximo vertice?
	if (prox == -1)
		return;
	
	Djikstra(grafo, prox, dist, visitado);
	
}

void Djikstra_init (grafo_T *grafo, int inicio){
	int dist[grafo->n_vertices], visitado[grafo->n_vertices], i;
	
	for (i = 0; i < grafo->n_vertices; i++){
		dist[i] = 1000000;
		visitado[i] = 0;
	}
	
	dist[inicio] = 0;
	
	
	Djikstra(grafo, inicio, dist, visitado);
	
	for (i = 0; i < grafo->n_vertices; i++){
		printf("%d: %d\n", i, dist[i]);
	}
}

//-----------------------------------------------------------------------------------

void Ord_Topologica (grafo_T *grafo, int vertice, int visitado[], int pilha[]){
	no_lista_adj_P no = grafo->vetorListas[vertice].cabeca;
	int i;
	
	visitado[vertice] = 1;
	
	while (no != NULL){
		if (visitado[no->vertice] == 0){
			Ord_Topologica(grafo, no->vertice, visitado, pilha);
		}
		no = no->prox;
	}
	
	for (i = 0; i < grafo->n_vertices; i++){
		if (pilha[i] == -1){
			pilha[i] = vertice;
			break;
		}
	}
}

void Ord_Topologica_init (grafo_T *grafo){
	int i, visitado[grafo->n_vertices], pilha[grafo->n_vertices];
	
	for (i = 0; i < grafo->n_vertices; i++){
		visitado[i] = 0;
		pilha[i] = -1;
	}
	
	for (i = 0; i < grafo->n_vertices; i++){
		if (visitado[i] == 0){
			Ord_Topologica(grafo, i, visitado, pilha);
		}
	}
	
	for (i = grafo->n_vertices - 1; i >= 0; i--){
		printf("%d: %d\n", grafo->n_vertices - i, pilha[i]);
	}
}

//-------------------------------------------------------------------------------

void menu () {
	int seletor, var1, var2, var3;
	char varchar1;
	grafo_P grafo = NULL;
	while (1){
		do {
			printf ("=======MENU=======\n");
			printf ("1. Criar grafo\n");
			printf ("2. Inserir vertice no grafo\n");
			printf ("3. Mostrar grafo\n");
			printf ("4. Destruir grafo\n");
			printf ("5. Busca Profundidade\n");
			printf ("6. Djikstra\n");
			printf ("7. Ordenacao Topologica\n");
			printf ("8. Sair\n");
			printf ("==================\n\n");
			printf ("Escolha uma opcao: ");
			scanf("%d", &seletor);
			system("cls");
		} while (seletor < 1 || seletor > 8);
		
		switch (seletor){
			case 1:
				if (grafo){
					printf("Grafo ja existente, destrua-o antes de criar outro.");
					getch();
					system("cls");
					break;
				}
				do{
					printf("Digite o tipo do grafo:\n0- Nao direcionado\n1- Nao direcionado com peso\n2- Direcionado\n3- Direcionado com peso\n>");
					scanf("%d", &var1);
				} while (var1 < 0 || var1 > 3);
				
				printf("\n\nDigite o numero de vertices do grafo: \n>");
				scanf("%d", &var2);
				
				grafo = cria_grafo(var2, var1);
				system("cls");
				break;
			
			case 2:
				if (!grafo){
					printf("Grafo nao existe, crie-o antes de tentar inserir um vertice.");
					getch();
					system("cls");
					break;
				}
				do{
					printf("Modelo: origem->destino\ndestino: vertice que sera incluido e sera vizinho do vertice \"origem\"");
					if (grafo->tipo_grafo == 1 || grafo->tipo_grafo == 3) {
						printf("\n\nDigite o vertice origem: ");
						scanf("%d", &var1);
						printf("\nDigite o vertice destino: ");
						scanf("%d", &var2);
						printf("\nDigite o peso do caminho: ");
						scanf("%d", &var3);
						
						insere(grafo, var1, var2, var3);
					}
					else {
						printf("\nDigite o vertice origem: ");
						scanf("%d", &var1);
						printf("\nDigite o vertice destino: ");
						scanf("%d", &var2);
						
						insere(grafo, var1, var2, 1);
					}
					
					printf("\n\nDeseja inserir mais um vertice? [S ou N]\n>");
					do{
						varchar1 = getch();
					} while (toupper(varchar1) != 'S' && toupper(varchar1) != 'N');
					system("cls");
				} while (toupper(varchar1) == 'S');
				break;
			
			case 3:
				imprime(grafo);
				getch();
				system("cls");
				break;
				
			case 4:
				if (!grafo){
					printf("Grafo nao existe, crie-o antes de tentar destrui-lo.");
					getch();
					system("cls");
					break;
				}
				mataGrafo(grafo);
				grafo = NULL;
				break;
				
			case 5:
				if (!grafo){
					printf("Grafo nao existe, crie-o antes de tentar realizar uma operacao.");
					getch();
					system("cls");
					break;
				}
				printf("Qual o ponto de partida?\n>");
				scanf("%d", &var1);
				system("cls");
				printf("Ponto de partida: %d\n\n", var1);
				busca_profundidade_init(grafo, var1);
				getch();
				system("cls");
				break;
			
			case 6:
				if (!grafo){
					printf("Grafo nao existe, crie-o antes de tentar realizar uma operacao.");
					getch();
					system("cls");
					break;
				}
				printf("Qual o ponto de partida?\n>");
				scanf("%d", &var1);
				system("cls");
				printf("Ponto de partida: %d\n\n", var1);
				Djikstra_init(grafo, var1);
				getch();
				system("cls");
				break;
				
			case 7:
				if (!grafo){
					printf("Grafo nao existe, crie-o antes de tentar realizar uma operacao.");
					getch();
					system("cls");
					break;
				}
				Ord_Topologica_init(grafo);
				getch();
				system("cls");
				break;
				
			case 8:
				if (grafo)
					mataGrafo(grafo);
				exit(1);
		}
	}
}

int main(){
	/*grafo_P undir_graph = cria_grafo(5, 1);
    grafo_P dir_graph = cria_grafo(5, 2);
    insere(undir_graph, 0, 1, 1);
    insere(undir_graph, 0, 4, 2);
    insere(undir_graph, 1, 2, 3);
    insere(undir_graph, 1, 3, 4);
    insere(undir_graph, 1, 4, 5);
    insere(undir_graph, 2, 3, 6);
    insere(undir_graph, 3, 4, 7);

    insere(dir_graph, 0, 1, 1);
    insere(dir_graph, 0, 4, 1);
    insere(dir_graph, 1, 2, 1);
    insere(dir_graph, 1, 3, 1);
    insere(dir_graph, 1, 4, 1);
    insere(dir_graph, 2, 3, 1);
    insere(dir_graph, 3, 4, 1);

    printf("\nUNDIRECTED GRAPH");
    imprime(undir_graph);
    mataGrafo(undir_graph);

    printf("\n\nDIRECTED GRAPH");
    imprime(dir_graph);
    mataGrafo(dir_graph);

    return 0;*/
    menu();
}
