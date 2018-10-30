#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Processo.h"
#include "fila.h"
#include <sys/time.h>

#define MEM 200
#define PRIORIDADE 10
#define TEMPO_CPU 500 //Tempo em milisegundos
#define TEMPO_ESPERA 200 //Tempo em que o processo irá espera a i/o

enum contexto{
    Pronto,
    Espera,
    Execucao
};

//Struct para passar dados para a thread que irá escalonar os processos
typedef struct data{
    Processo* p;
    int *encerrar;
    int *tipo;
    int *t_min;
    int *t_max;
    int *memoria;
    Fila *proc_proces;
    Fila *espera;
} Data;

//Esqueleto das funções que irão virar threads
void* escalonar(void* dados);

void* esperar(void* dados);

void* final(void* dados);

//Função Principal
int main(void) {

    printf("Simulador de Escalonamento\n\n");

    printf("Para encerrar o programa digite \'t\' e aperte enter\n");

    //Identificador das threads
    pthread_t so,fim,esp;

    //Possivelmente vai ser mudado para o algoritimo de memoria, e n ta sendo usado
    int memoria = MEM; //Aloca Memoria

    //Filas utilizadas
    Fila *espera = fila_cria();
    Fila *proc_proces = fila_cria();//De processos criados

    double quantum = 100;

    //Variavel para encerrar os loops encerrando assim o programa
    int encerrar = 1;

    int i = 0;

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

    printf("Digite o tempo que sera executado o processo: ");
    scanf("%lf", &quantum);

    //Limpa o buffer do teclado
    fflush(stdin);

    //Atribuido os valores para depois passar os dados
    Data data;
    data.encerrar = &encerrar;
    data.tipo = &tipo;
    data.t_min = &t_min;
    data.t_max = &t_max;
    data.proc_proces = proc_proces;
    data.espera = espera;
    data.memoria = &memoria;

    Data data_esp;
    data_esp.espera = espera;
    data_esp.encerrar = &encerrar;
    data_esp.proc_proces = proc_proces;
    data_esp.memoria = &memoria;

    //Valores necessarios para medições
    double tmp_ini = 0 ,tmp_fim = 0 , tmp_ocisoso = 0;

    struct timeval t_ini, t_fim;

    //Criação de threads para o funcionamento de todo o sistema
    pthread_create(&so, NULL, escalonar, (void*) &data);

    pthread_create(&fim, NULL, final, (void*) &encerrar);

    pthread_create(&esp, NULL, esperar, (void*) &data_esp);

    i = 0;//Variavel para registrar o numero de processos processados.
    //Esse argumento do laço não está legal, pois se houver interrupções
    while(encerrar){

        //printf("%d\n", fila_vazia(proc_proces));

        //começa contar o tempo do processo em milisegundos
        gettimeofday(&t_ini,NULL);
        tmp_ini = (double) t_ini.tv_usec + ((double)t_ini.tv_sec * (1000000.0));

        //Verifica se o processador está ocioso e contabiliza o tempo total
        if(fila_vazia(proc_proces)){
            while (fila_vazia(proc_proces) && encerrar) {
            }
            // operação para receber e converter os valores
            gettimeofday(&t_fim,NULL);
            tmp_fim = (double) t_fim.tv_usec + ((double)t_fim.tv_sec * (1000000.0));
            tmp_ocisoso += (tmp_fim - tmp_ini) / 1000;
        }

        //Talvez não seja necessario.
        if(!fila_vazia(proc_proces)){

            //Retira o processo que vai ser processado da fila de processos
            Processo *aux = fila_retira(proc_proces);
            printf("Num. Processo : %d\n", i);

            set_contexto(aux, Execucao);

            while (((tmp_fim - tmp_ini) / 1000 < get_temp(aux)) &&
                    (tmp_fim - tmp_ini) / 1000 < quantum){
                gettimeofday(&t_fim,NULL);
                tmp_fim = (double)  t_fim.tv_usec + ((double) t_fim.tv_sec * (1000000.0));
            }

            //Se a sinalização não for ativada deve ser liberado o espaço de memoria
            if((tmp_fim - tmp_ini) / 1000 < get_temp(aux)){
                set_contexto(aux, Pronto);
                set_temp(aux, get_temp(aux) - (tmp_fim - tmp_ini));
                fila_insere(proc_proces, aux);
                i--;
            }else{
                memoria += get_memoria(aux);
                libera_processo(aux);
            }

            printf("\n------------------------\n");//Divisor entre processos

            i++;
        }

    }

    pthread_join(so, NULL);//Espera a o so terminar seu trabalho
    pthread_join(esp, NULL);

    printf("\nTempo total Ocioso(milissegundos): %f\n",tmp_ocisoso);
    printf("Numero total de processos: %d\n", i - 1);

    return 0;
}

void* escalonar(void* dados){

    //Recebe os valores passados
    Data *data;
    data = (Data*) dados;

    int *encerrar = data->encerrar;
    int *tipo = data->tipo;
    int *t_min = data->t_min;
    int *t_max = data->t_max;
    Fila *proc_proces = data->proc_proces;
    Fila *espera = data->espera;
    int *memoria = data->memoria;

    //Laço so fica ativo até que que seja indicado pelo usuario o tempo de finalização
    while(*encerrar){

        //Simula a "chegada" de processos
        Processo *esc = aloca_processo();

        srand(time(NULL));//Redefine a semente da função rand

        set_contexto(esc, rand()%2);
        set_prioridade(esc, rand()%PRIORIDADE);
        set_temp(esc, rand()%TEMPO_CPU);
        set_memoria(esc, 1 + rand()%50);

        printf("Contexto definido : %d\n", get_contexto(esc));

        //Verificar se possui espaço de memoria com o algoritimo de memoria

        while(get_memoria(esc) > *memoria){
            //Fica parado até q o espaço de memoria esteja liberado
        }

        *memoria -= get_memoria(esc);

        printf("Espaço na memoria: %d\n", *memoria);

        if(get_contexto(esc) == Pronto){
            fila_insere(proc_proces, esc);
        }else{
            fila_insere(espera, esc);
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
void* esperar(void* dados){

    Data *data_esp = (void*) dados;
    Fila *espera = data_esp->espera;
    int *encerra = data_esp->encerrar;
    Fila *proc_proces = data_esp->proc_proces;

    while(*encerra){
        if(!fila_vazia(espera)){
            srand(time(NULL));
            usleep(rand() % TEMPO_ESPERA);
            fila_insere(proc_proces, fila_retira(espera));
        }
    }

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
