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

int main(void) {

    printf("Simulador de Escalonamento\n");

    pthread_t so;

    int memoria[MEM];

    int sinal = 0;

    int i;
    for(i = 0;i < MEM;i++){
        memoria[i] = -1;
    }

    int num_proc = 1;

    Processo *proc = aloca_processo(num_proc);

    for(i = 0;i < num_proc;i++){

        long tmp_init,tmp_fim;
        tmp_fim = tmp_init = time(NULL);

        while ((difftime(tmp_fim, tmp_init) < get_temp(proc, i)) && !sinal) {



            tmp_fim = time(NULL);
        }

    }

    return 0;
}
