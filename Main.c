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

typedef struct data{
    Processo* p;
    int *prioridade;
    int *encerrar;
    int *sinal;
    int *id_proc;
    Fila *proc_proces;
    Fila *interrompido;
} Data;

void* escalonar(void* dados);

void* espera(void* dados);

void* final(void* dados);

int main(void) {

    printf("Simulador de Escalonamento\n");

    pthread_t so,fim;

    int memoria[MEM]; //Aloca Memoria

    Fila *chegada = fila_cria();
    Fila *proc_proces = fila_cria();
    Fila *interrompido = fila_cria();

    int sinal = 0; //Sinalização para o "Processador"
    int id_proc = -1; //ID do processo que fez a sinalização
    //Variavel para encerrar o loop inicial
    int encerrar = 1;

    //Inicializa toda a memoria como -1
    int i;
    for(i = 0;i < MEM;i++){
        memoria[i] = -1;
    }

    printf("Numero de Processos(%d espacos de memoria): \n", MEM);

    //Alocar Memoria ligando ao outro programa

    printf("Tempo de chegada dos processos:\n");
    printf("0 - Zero\n1 - Aleatorio\n3 - Fixo\n");

    //Aloca memoria para todos processos

    int prioridade = 0;

    Data data;
    data.prioridade = &prioridade;
    data.encerrar = &encerrar;
    data.sinal = &sinal;
    data.id_proc = &id_proc;
    data.proc_proces = proc_proces;
    data.interrompido = interrompido;

    double tmp_ini = 0 ,tmp_fim = 0 , tmp_ocisoso = 0;

    struct timeval t_ini, t_fim;

    pthread_create(&so, NULL, escalonar, (void*) &data);

    pthread_create(&fim, NULL, final, (void*) &encerrar);

    i = 0;
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

    pthread_join(so, NULL);

    printf("Tempo total Ocioso(milissegundos): %f\n",tmp_ocisoso);

    return 0;
}

void* escalonar(void* dados){

    Data *data;
    data = (Data*) dados;

    int *encerrar = data->encerrar;
    int *sinal = data->sinal;
    int *prioridade_cpu = data->prioridade;
    Fila *proc_proces = data->proc_proces;
    Fila *interrompido = data->interrompido;

    while(*encerrar){

        //Simula a "chegada" de processos
        Processo *esc = aloca_processo();

        srand(time(NULL));

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

        usleep(800000);//tempo em microsegundos para mudar o srand

    }

    return NULL;
}

//função para fazer os processos ficarem em espera pelo tempo determinado
void* espera(void* dados){



    return NULL;
}

void* final(void* dados){

    int *final = (int*) dados;

    getchar();

    *final = 0;

    return NULL;
}
