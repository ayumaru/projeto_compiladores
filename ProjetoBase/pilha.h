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
int remove_elemento_tipo_pilha(pilha_tipo_enum*pilha_tipo);

void destruir_pilha_tipos(pilha_tipo_enum *pilha_tipo); 

int verificador_tipos(pilha_tipo_enum *pilha_tipo, tipos_enum esperado, tipos_enum resultado);
int verifica_atributo_tipos(pilha_tipo_enum *pilha_tipo);
int verifica_tipos_expressao(pilha_tipo_enum *pilha_tipo, tipos_enum esperado);

void andar_pilha_tipos(pilha_tipo_enum *pilha_tipo); // parece servir para debug

/* -------------------- FIM PILHA DE TIPOS -------------------- */

/* -------------------- COMECO PILHA DE ROTULOS -------------------- */

typedef struct nodo_rotulo{
    char rotulo[TAM_ROTULO];
    struct nodo_rotulo *prox;
} nodo_rotulo;

typedef struct pilha_rotulo{
    struct nodo_rotulo *ultimo;
    int atual;
} pilha_rotulo;

char *insere_rotulo(pilha_rotulo *p_rotulo);
int remove_rotulo(pilha_rotulo *p_rotulo);

void destroi_pilha_rotulo(pilha_rotulo *p_rotulo);

void andar_pilha_rotulo(pilha_rotulo *p_rotulo); // serve de debug


/* -------------------- FIM PILHA DE ROTULOS -------------------- */


/* -------------------- COMECO PILHA DE DESLOCAMENTO -------------------- */

typedef struct nodo_deslocamento {
    int desloc;
    struct nodo_deslocamento *prox;
} nodo_deslocamento;

typedef struct pilha_deslocamento{
    struct nodo_deslocamento *ultimo;
    int nv_lexico;
} pilha_deslocamento;

void pilha_insere_deslocamento(pilha_deslocamento *p_deslocamento, int quantidade);
int pilha_remove_deslocamento(pilha_deslocamento *p_deslocamento);

void destroi_pilha_deslocamento(pilha_deslocamento *p_deslocamento);

int andar_pilha_deslocamento(pilha_deslocamento *p_deslocamento); // serve de debug


/* -------------------- FIM PILHA DE DESLOCAMENTO -------------------- */

/* -------------------- COMECO PILHA DE PROCEDIMENTOS ROTULO -------------------- */

typedef struct nodo_procedimento_rotulo{
    struct nodo_procedimento_rotulo *prox;
    char rotulo[TAM_ROTULO];
} nodo_procedimento_rotulo;

typedef struct pilha_procedimento_rotulo{
    int atual;
    struct nodo_procedimento_rotulo *ultimo;
} pilha_procedimento_rotulo;

char *insere_pilha_procedimento_rotulo(pilha_procedimento_rotulo *p_procrot);
void destroi_pilha_procedimento_rotulo(pilha_procedimento_rotulo *p_procrot);

/* -------------------- FIM PILHA DE PROCEDIMENTOS ROTULO -------------------- */

/* -------------------- COMECO PILHA DE PROCEDIMENTOS SIMBOLOS -------------------- */

typedef struct nodo_proc_simb{
    struct nodo_proc_simb *prox;
    tab_simbolo *simbolo;
} nodo_proc_simb;

typedef struct pilha_proc_simb{
    nodo_proc_simb *ultimo;
    int atual;
} pilha_proc_simb;

void insere_pilha_simb(pilha_proc_simb *p_proc_sb, tab_simbolo *simb);
int remove_pilha_simb(pilha_proc_simb *p_proc_sb);

void destroi_pilha_simb(pilha_proc_simb *p_proc_sb);


#endif