/* Processo.c */

#include <stdio.h>
#include <stdlib.h>
#include "Processo.h"

struct processo{
    int prioridade;
    int temp_cpu;
    int estado;
    int finalizado;
    int memoria;
};

enum contexto{
    Pronto,
    Espera,
    Execucao
};

Processo* aloca_processo(){

    Processo *proce = malloc(sizeof(Processo));

    return proce;
}

int get_temp(Processo *p){
    return p->temp_cpu;
}

void set_temp(Processo *p, int tempo){
    p->temp_cpu = tempo;
}

int get_contexto(Processo *p){
    return p->estado;
}

void set_contexto(Processo *p, int contexto){
    p->estado = contexto;
}

void set_prioridade(Processo *p, int prioridade){
    p->prioridade = prioridade;
}

int get_prioridade(Processo *p){
    return p->prioridade;
}

Processo* get_processo(Processo *p, int pos){
    return &p[pos];
}

void libera_processo(Processo *p){
    free(p);
}

int get_memoria(Processo *p){
    return p->memoria;
}

void set_memoria(Processo *p, int memoria){
    p->memoria = memoria;
}
