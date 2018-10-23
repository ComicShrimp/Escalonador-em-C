#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Processo.h"
#include "fila.h"

#define MEM 200

enum contexto{
    Pronto,
    Espera,
    Execucao
};

typedef struct data{
    Processo* p;
    int num_proce;
    int *sinal;
    int *id_proc;
    Fila *proc_proces;
} Data;

void* escalonar(void* dados);

int main(void) {

    printf("Simulador de Escalonamento\n");

    pthread_t so;

    int memoria[MEM]; //Aloca Memoria

    Fila *proc_proces = fila_cria();

    int sinal = 0; //Sinalização para o "Processador"
    int id_proc = -1; //ID do processo que fez a sinalização

    //Inicializa toda a memoria como -1
    int i;
    for(i = 0;i < MEM;i++){
        memoria[i] = -1;
    }

    printf("Numero de Processos(%d espacos de memoria): \n", MEM);

    printf("Tempo de chegada dos processos:\n");
    printf("0 - Zero\n1 - Aleatorio\n3 - Fixo\n");

    //Numero total de proocessos
    int num_proc = 1;

    //Aloca memoria para todos processos
    Processo *proc = aloca_processo(num_proc, &sinal, &id_proc);

    Data data;
    data.num_proce = num_proc;
    data.p = proc;
    data.sinal = &sinal;
    data.id_proc = &id_proc;
    data.proc_proces = proc_proces;

    pthread_create(&so, NULL, escalonar, (void*) &data);

    long tmp_ocisoso = 0;

    //Esse argumento do laço não está legal, pois se houver interrupções
    for(i = 0;i < num_proc;i++){

        long tmp_init,tmp_fim;
        tmp_fim = tmp_init = time(NULL);

        //Verifica se o processador está ocioso e contabiliza o tempo total
        if(fila_vazia(proc_proces)){
            while (fila_vazia(proc_proces)) {
                //printf("Ocioso\n");
            }

            tmp_ocisoso += difftime(time(NULL), tmp_init);

        }else{

            //Esse laço não pode ter sleep, pois ele deve ser capaz de ser
            //interrompido pela sinalização.
            while ((difftime(tmp_fim, tmp_init) < get_temp(proc, i)) && !sinal) {



                tmp_fim = time(NULL);
            }
        }

    }

    pthread_join(so, NULL);

    return 0;
}

void* escalonar(void* dados){

    Data *data;
    data = (Data*) dados;

    int n_proc = data->num_proce;
    int id_proc = data->id_proc;
    int sinal = data->sinal;
    Processo *p = data->p;
    Fila *proc_proces = data->proc_proces;

    int i;
    for (i = 0; i < n_proc; i++) {
        /* code */
    }

    /*
        Resto do programa
    */

    return NULL;
}
