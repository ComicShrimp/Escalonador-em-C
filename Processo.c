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

Processo* aloca_processo(int num_proc){

    Processo *proce = malloc(num_proc * sizeof(Processo));

    return proce;
}

int get_temp(Processo *p, int pos){
    return p[pos].temp_cpu;
}
