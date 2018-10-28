#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Processo.h"
#include "fila.h"
#include <sys/time.h>

#define MEM 200
#define PRIORIDADE 10
#define TEMPO_CPU 5000 //Tempo em milisegundos

enum contexto{
    Pronto,
    Espera,
    Execucao
};

//Struct para passar dados para a thread que irá escalonar os processos
typedef struct data{
    Processo* p;
    int *prioridade;
    int *encerrar;
    int *sinal;
    int *tipo;
    int *t_min;
    int *t_max;
    Fila *proc_proces;
    Fila *interrompido;
} Data;

//Esqueleto das funções que irão virar threads
void* escalonar(void* dados);

void* espera(void* dados);

void* final(void* dados);

//Função Principal
int main(void) {

    printf("Simulador de Escalonamento\n\n");

    printf("Para encerrar o programa digite \'t\' e aperte enter\n");

    //Identificador das threads
    pthread_t so,fim;

    //Possivelmente vai ser mudado para o algoritimo de memoria, e n ta sendo usado
    int memoria[MEM]; //Aloca Memoria

    //Filas utilizadas
    Fila *chegada = fila_cria();
    Fila *proc_proces = fila_cria();//De processos criados
    Fila *interrompido = fila_cria();//De processos que foram interrompidos

    int sinal = 0; //Sinalização para o "Processador"

    //Variavel para encerrar os loops encerrando assim o programa
    int encerrar = 1;

    //Talvez não seja necessario, tem q ver como será a questão da memoria
    //Inicializa toda a memoria como -1
    int i;
    for(i = 0;i < MEM;i++){
        memoria[i] = -1;
    }

    //Alocar Memoria ligando ao outro programa

    int tipo = 2;//Tipo default para não ocorrer erros
    int t_min, t_max;
    t_min = t_max = 0;//Valor padrão para não ter erros

    //Textos de configuração do usuario
    printf("Atenção, com o tempo 0, a aleatoriedade é comprometida !!!\n");
    printf("Tempo de chegada dos processos:\n");
    printf("0 - Zero\n1 - Aleatorio\n2 - Fixo\n\n");
    scanf("%d", &tipo);

    if(tipo == 1){
        printf("Tempo em milissegundos\n");
        printf("Tempo minimo: ");
        scanf("%d", &t_min);

        printf("Tempo maximo: ");
        scanf("%d", &t_max);
    }

    //Limpa o buffer do teclado
    fflush(stdin);

    //Valor default da prioridade para não dar erros
    int prioridade = 0;

    //Atribuido os valores para depois passar os dados
    Data data;
    data.prioridade = &prioridade;
    data.encerrar = &encerrar;
    data.sinal = &sinal;
    data.tipo = &tipo;
    data.t_min = &t_min;
    data.t_max = &t_max;
    data.proc_proces = proc_proces;
    data.interrompido = interrompido;

    //Valores necessarios para medições
    double tmp_ini = 0 ,tmp_fim = 0 , tmp_ocisoso = 0;

    struct timeval t_ini, t_fim;

    //Criação de threads para o funcionamento de todo o sistema
    pthread_create(&so, NULL, escalonar, (void*) &data);

    pthread_create(&fim, NULL, final, (void*) &encerrar);

    i = 0;//Variavel para registrar o numero de processos processados.
    //Esse argumento do laço não está legal, pois se houver interrupções
    while(encerrar){

        sinal = 0;

        printf("%d\n", fila_vazia(proc_proces));

        //começa contar o tempo do processo em milisegundos
        gettimeofday(&t_ini,NULL);
        tmp_ini = (double) t_ini.tv_usec + ((double)t_ini.tv_sec * (1000000.0));

        //Verifica se o processador está ocioso e contabiliza o tempo total
        if(fila_vazia(proc_proces)){
            while (fila_vazia(proc_proces) && encerrar) {
                //printf("Ocioso\n");
            }
            // operação para receber e converter os valores
            gettimeofday(&t_fim,NULL);
            tmp_fim = (double) t_fim.tv_usec + ((double)t_fim.tv_sec * (1000000.0));
            tmp_ocisoso += (tmp_fim - tmp_ini) / 1000;
        }

        //Talvez não seja necessario.
        if(!fila_vazia(proc_proces)){

            //Esse laço não pode ter sleep, pois ele deve ser capaz de ser
            //interrompido pela sinalização.
            Processo *aux;
            if(fila_vazia(interrompido)){
                //Retira o processo que vai ser processado da fila de processos
                aux = fila_retira(proc_proces);
            }else{
                //Retira o processo da fila e processos interrompidos
                aux = fila_retira(interrompido);
            }
            prioridade = get_prioridade(aux);
            printf("%d - num: %d\n", prioridade,i);

            while ((tmp_fim - tmp_ini) / 1000 < get_temp(aux) && !sinal){
                gettimeofday(&t_fim,NULL);
                tmp_fim = (double)  t_fim.tv_usec + ((double) t_fim.tv_sec * (1000000.0));
            }

            //Se a sinalização não for ativada deve ser liberado o espaço de memoria
            if(!sinal){
                libera_processo(aux);
            }else{
                set_temp(aux, get_temp(aux) - (tmp_fim - tmp_ini));
                fila_insere(interrompido, aux);
            }

            printf("\n------------------------\n");//Divisor entre processos

        }

        i++;
    }

    pthread_join(so, NULL);//Espera a o so terminar seu trabalho

    printf("\nTempo total Ocioso(milissegundos): %f\n",tmp_ocisoso);
    printf("Numero total de processos: %d\n", i - 1);

    return 0;
}

void* escalonar(void* dados){

    //Recebe os valores passados
    Data *data;
    data = (Data*) dados;

    int *encerrar = data->encerrar;
    int *sinal = data->sinal;
    int *tipo = data->tipo;
    int *prioridade_cpu = data->prioridade;
    int *t_min = data->t_min;
    int *t_max = data->t_max;
    Fila *proc_proces = data->proc_proces;
    Fila *interrompido = data->interrompido;

    //Laço so fica ativo até que que seja indicado pelo usuario o tempo de finalização
    while(*encerrar){

        //Simula a "chegada" de processos
        Processo *esc = aloca_processo();

        srand(time(NULL));//Redefine a semente da função rand

        set_contexto(esc, rand()%3);
        set_prioridade(esc, rand()%PRIORIDADE);
        set_temp(esc, rand()%TEMPO_CPU);

        printf("Contexto definido : %d\n", get_contexto(esc));

        if((*prioridade_cpu < get_prioridade(esc)) && fila_vazia(interrompido)){

            //*sinal = 1;

            //Inserir no topo da fila e sinalizar
            //Possivel problema, tomar o lugar de um processo de maior prioridade
            //que está na fila ou gerar interrupções infinitas

            //Apenas para sinalizar ao usuario que entrou nesse local
            //printf("Prioridade\n");

            //Fazer função para colocar-lo como o primeiro da fila.
        }

        //Verificar se possui espaço de memoria com o algoritimo de memoria

        if(get_contexto(esc) == Pronto){
            fila_insere(proc_proces, esc);
        }

        if(*tipo == 1){
            usleep((*t_min * 1000) + (rand() % (*t_max * 1000)));
        }else if(*tipo == 2){
            usleep(800000);
        }

    }

    return NULL;
}

//função para fazer os processos ficarem em espera pelo tempo determinado
void* espera(void* dados){



    return NULL;
}

void* final(void* dados){

    int *final = (int*) dados;

    while (getchar() != 't') {
        //Vai ficar aqui até que seja pressionado a tecla t
    };

    *final = 0;

    return NULL;
}
