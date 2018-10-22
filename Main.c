#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Processo.h"

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
} Data;

void* escalonar(void* dados);

int main(void) {

    printf("Simulador de Escalonamento\n");

    pthread_t so;

    int memoria[MEM];

    int sinal = 0;
    int id_proc = -1;

    int i;
    for(i = 0;i < MEM;i++){
        memoria[i] = -1;
    }

    int num_proc = 1;

    Processo *proc = aloca_processo(num_proc, &sinal, &id_proc);

    Data data;
    data.num_proce = num_proc;
    data.p = proc;
    data.sinal = &sinal;
    data.id_proc = &id_proc;

    pthread_create(&so, NULL, escalonar, (void*) &data);

    for(i = 0;i < num_proc;i++){

        long tmp_init,tmp_fim;
        tmp_fim = tmp_init = time(NULL);

        while ((difftime(tmp_fim, tmp_init) < get_temp(proc, i)) && !sinal) {



            tmp_fim = time(NULL);
        }

    }

    return 0;
}

void* escalonar(void* dados){

    Data *data;
    data = (Data*) dados;

    int n_proc = data->num_proce;
    Processo *p = data->p;
    int id_proc = data->id_proc;
    int sinal = data->sinal;

    /*
        Resto do programa
    */

    return NULL;
}
