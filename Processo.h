/* Processo.h */

typedef struct processo Processo;

Processo* aloca_processo(int num_proc, int *sinal, int *id_proc);

int get_temp(Processo *p, int pos);

//O tempo requirido é em milisegundos
void set_temp(Processo *p, int pos, int tempo);

int get_contexto(Processo *p, int pos);

void set_contexto(Processo *p, int pos, int contexto);
