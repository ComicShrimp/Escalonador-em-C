/* Processo.h */

typedef struct processo Processo;

Processo* aloca_processo();

int get_temp(Processo *p);

//O tempo requirido é em milisegundos
void set_temp(Processo *p, int tempo);

int get_contexto(Processo *p);

void set_contexto(Processo *p, int contexto);

void set_prioridade(Processo *p, int prioridade);

int get_prioridade(Processo *p);

Processo* get_processo(Processo *p, int pos);

void libera_processo(Processo *p);

int get_memoria(Processo *p);

void set_memoria(Processo *p, int memoria);
