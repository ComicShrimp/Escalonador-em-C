/* fila.c */
#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct lista {	// estrutura lista para implementa��o da lista
	float info;
	struct lista* prox;
};

struct fila {
	Lista* ini;	// ponteiro para o n� do in�cio da fila
	Lista* fim;	// ponteiro para o n� do fim da fila
};

Fila* fila_cria(void) {						// cria uma fila, alocando memória para ela
	Fila* f = (Fila*) malloc(sizeof(Fila));	// aloca memória do tamanho de uma fila
	f->ini = f->fim = NULL;					// inicializa os dois ponteiros (ini e fim) como nulos, pois a fila est� vazia
	return f;								// retorna o ponteiro que aponta para o endere�o alocado
}

void fila_insere(Fila* f, float v) {			// insere um elemento (n�) no fim da fila
	Lista* n = (Lista*) malloc(sizeof(Lista));	// aloca memória para o novo n�
	n->info = v;								// guarda no n� o valor passado como argumento (float v)
	n->prox = NULL;								// faz o pr�ximo do n� ser nulo, pois ser� o �ltimo
	if(f->fim != NULL)							// verifica se a fila n�o est� vazia...
		f->fim->prox = n;							// se n�o estiver, apenas faz o atual n� do fim apontar para o novo n� criado
	else										// caso contr�rio...
		f->ini = n;									// j� que a fila est� vazia, o novo n� ser� tanto o �ltimo como o primeiro. Ent�o, � preciso atualizar "ini" tamb�m
	f->fim = n;									// faz o ponteiro "fim" apontar para o novo n�, j� que ele ser� mesmo o �ltimo da fila
}

int fila_vazia(Fila* f) {		// verifica se a fila est� vazia ou n�o
	return (f->ini == NULL);	// retorna o resultado (0 ou 1) da opera��o l�gica de igualdade entre o ponteiro "ini" e NULL (se for NULL, a fila est� vazia: retorna 1)
}

float fila_retira(Fila* f) {		// remove o primeiro elemento da fila (FIFO)
	Lista* t;						// ponteiro auxiliar para guardar o n� que vai ser removido
	float v;
	if(fila_vazia(f)) {				// verifica se a fila est� vazia. Se estiver...
		printf("Fila vazia!\n");		// imprime mensagem de fila vazia
		exit(1);						// e encerra o programa
	}								// se n�o estiver, continua a fun��o
	t = f->ini;						// guarda temporariamente o endere�o do primeiro n�
	v = t->info;					// guarda temporariamente a informa��o do primeiro n�
	f->ini = t->prox;				// atualiza ponteiro "ini" para o pr�ximo elemento da fila
	if(f->ini == NULL)				// se a fila tiver ficado vazia ap�s a remo��o...
		f->fim = NULL;					// atualiza tamb�m o ponteiro do fim para NULL
	free(t);						// libera o endere�o do n� removido
	return v;						// retorna o valor do n� removido (float)
}

void fila_libera(Fila* f) {		// libera a memória alocada para a fila
	Lista* q = f->ini;			// ponteiro auxiliar para come�ar a percorrer a lista da fila desde o in�cio
	while(q != NULL) {			// la�o para percorrer a lista da fila
		Lista* t = q->prox;			// guarda o endere�o do pr�ximo n� para que o atual seja liberado sem perder a lista
		free(q);					// libera o n� atual
		q = t;						// aponta para o pr�ximo n� (que estava guardado em "t")
	}
	free(f);					// libera, por fim, o ponteiro da fila inteira
}

void fila_imprime(Fila* f) {			// imprime todos os elementos da fila
	if(!fila_vazia(f)) {				// verifica se a fila n�o est� vazia. Se n�o estiver...
		Lista* q = f->ini;					// ponteiro auxiliar para percorrer a lista da fila desde o in�cio
		while(q != NULL) {					// la�o para percorrer a lista at� o fim
			printf("%.2f ", q->info);			// imprime informa��o (valor) do n� atual
			q = q->prox;						// atualiza "q" para o pr�ximo n� da lista
		}
		printf("\n");						// imprime nova linha ao fim da impress�o da fila
	} else								// caso contr�rio, se estiver vazia...
		printf("Fila vazia!\n");			// imprime mensagem indicando que est� vazia
}
