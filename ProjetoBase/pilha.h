#ifndef __PILHA__
#define __PILHA__

#include "funcoes.h"


/* -------------------- COMECO PILHA DE TIPOS -------------------- */
// Pilha de tipos
typedef struct nodo_tipos_enum{
    tipos_enum tipo;
    struct nodo_tipos_enum *prox;
} nodo_tipos_enum;

typedef struct pilha_tipo_enum{
    int tam;
    nodo_tipos_enum *ultimo;
} pilha_tipo_enum;


// funcoes para pilha de tipos
void insere_elemento_tipo_pilha(pilha_tipo_enum *pilha_tipo, tipos_enum tipo);
int remove_elemento_tipo_pilha( nodo_tipos_enum *pilha_tipo);

void destruir_pilha_tipos(pilha_tipo_enum *pilha_tipo); 

int verificador_tipos(pilha_tipo_enum *pilha_tipo, tipos_enum esperado, tipos_enum resultado);
int verifica_atributo_tipos(pilha_tipo_enum *pilha_tipo);
int verifica_tipos_expressao(pilha_tipo_enum *pilha_tipo, tipos_enum esperado);

void andar_pilha_tipos(pilha_tipo_enum *pilha_tipo); // parece servir para debug

/* -------------------- FIM PILHA DE TIPOS -------------------- */

/* -------------------- COMECO PILHA DE ROTULOS -------------------- */

#endif