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

void* espera(void* dados);

int main(void) {

    printf("Simulador de Escalonamento\n");

    pthread_t so;

    int memoria[MEM]; //Aloca Memoria

    Fila *proc_proces = fila_cria();
    Fila *interrompido = fila_cria();

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
    int num_proc = 4;

    //Aloca memoria para todos processos
    Processo *proc = aloca_processo(num_proc, &sinal, &id_proc);

    //Variavel para guardar o processo q  foi interrompido
    Processo *aux = aloca_processo(1, &sinal, &id_proc);

    Data data;
    data.num_proce = num_proc;
    data.p = proc;
    data.sinal = &sinal;
    data.id_proc = &id_proc;
    data.proc_proces = proc_proces;

    pthread_create(&so, NULL, escalonar, (void*) &data);

    long tmp_ocisoso = 0;//Averiguar pq está contando diretamente em segundos
    long tmp_init,tmp_fim;

    //Esse argumento do laço não está legal, pois se houver interrupções
    for(i = 0;i < num_proc;i++){

        printf("%d\n", fila_vazia(proc_proces));

        tmp_fim = tmp_init = time(NULL);

        //Verifica se o processador está ocioso e contabiliza o tempo total
        if(fila_vazia(proc_proces)){
            while (fila_vazia(proc_proces)) {
                //printf("Ocioso\n");
            }
            tmp_ocisoso += difftime(time(NULL), tmp_init);
        }

        //if(!fila_vazia(proc_proces)){

            //Esse laço não pode ter sleep, pois ele deve ser capaz de ser
            //interrompido pela sinalização.
            Processo* aux = fila_retira(proc_proces);//Retira o processo que acabou de ser processado
            printf("%d - num: %d\n", get_prioridade(aux, 0),i);
            while ((difftime(tmp_fim, tmp_init) < get_temp(aux, 0)) && !sinal) {

                tmp_fim = time(NULL);
            }

            printf("------------------------\n");//Divisor entre processos

            //Caso o sinal seja ativado, o processo que estava em execução será
            //guardado para ser usado na retomada.
            if(sinal){
                set_prioridade(aux, 0, get_prioridade(proc, i));
                set_contexto(aux, 0, Pronto);
                set_temp(aux, 0, difftime(get_temp(proc, i), tmp_fim));
            }

    //    }

    }

    pthread_join(so, NULL);

    printf("%ld\n",tmp_ocisoso);

    return 0;
}

void* escalonar(void* dados){

    Data *data;
    data = (Data*) dados;

    long temp_mili, temp_sec;// para fazer a conversão dos segundos

    int n_proc = data->num_proce;
    int *id_proc = data->id_proc;
    int *sinal = data->sinal;
    Processo *p = data->p;
    Fila *proc_proces = data->proc_proces;

    int i;
    for (i = 0; i < n_proc; i++) {

        srand(time(NULL));

        set_contexto(p, i, Pronto);
        set_prioridade(p, i, rand()%10);    //Prioridade vai de 0 a 10
        temp_mili = (rand()%5000);
        temp_sec = temp_mili/1000;
        set_temp(p, i, temp_sec);   //Tempo é em milisegundos

        //Se o contexto ja for de pronto, ja é inserido na lista de execução
        if(get_contexto(p, i) == Pronto){
            fila_insere(proc_proces, get_processo(p, i));
        }

        usleep(500000);//tempo em microsegundos

    }

    /*
        Resto do programa
    */

    return NULL;
}

//função para fazer os processos ficarem em espera pelo tempo determinado
void* espera(void* dados){



    return NULL;
}
