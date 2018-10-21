/* Processo.h */

typedef struct processo Processo;

Processo* aloca_processo(int num_proc);

int get_temp(Processo *p, int pos);
