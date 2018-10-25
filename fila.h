/* fila.h */
#include "Processo.h"

/* Tipos exportado */
typedef struct fila Fila;
typedef struct lista Lista;

/* Função cria
** Aloca memória para uma estrutura do tipo Fila e retorna o ponteiro
*/
Fila* fila_cria(void);

/* Função insere
** Insere um elemento no fim da Fila
*/
void fila_insere(Fila* f, Processo *v);

/* Função retira
** Remove o elemento do início da Fila
*/
Processo* fila_retira(Fila* f);

/* Função vazia
** Retorna um inteiro (0 ou 1), indicando se a Fila está vazia ou não
*/
int fila_vazia(Fila* f);

/* Função libera
** Libera a memória alocada para a Fila
*/
void fila_libera(Fila* f);

/* Função imprime
** Imprime todos os elementos da fila em ordem
*/
void fila_imprime(Fila* f);
