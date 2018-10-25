/* Processo.c */

#include <stdio.h>
#include <stdlib.h>
#include "Processo.h"

struct processo{
    int prioridade;
    int temp_cpu;
    int estado;
};

enum contexto{
    Pronto,
    Espera,
    Execucao
};

Processo* aloca_processo(int num_proc, int *sinal, int *id_proc){

    Processo *proce = malloc(num_proc * sizeof(Processo));

    return proce;
}

int get_temp(Processo *p, int pos){
    return p[pos].temp_cpu;
}

void set_temp(Processo *p, int pos, int tempo){
    p[pos].temp_cpu = tempo;
}

int get_contexto(Processo *p, int pos){
    return p[pos].estado;
}

void set_contexto(Processo *p, int pos, int contexto){
    p[pos].estado = contexto;
}

void set_prioridade(Processo *p, int pos, int prioridade){
    p[pos].prioridade = prioridade;
}

int get_prioridade(Processo *p, int pos){
    return p[pos].prioridade;
}

Processo* get_processo(Processo *p, int pos){
    return &p[pos];
}
