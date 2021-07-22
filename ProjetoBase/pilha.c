/*
# PARA REMOVER DEPOIS: nao entendi direito o funcionamento dessa
# melhorar a funcao error_handler com o nome da pilha que deu erro
*/

#include "debug.h"
#include "pilha.h"
#include "compilador.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* -------------------- COMECO FUNCOES DA PILHA DE TIPOS -------------------- */
/*
# Funcoes da pilha de tipos
# Criacao, destruicao, insercao
*/

void insere_elemento_tipo_pilha(pilha_tipo_enum *pilha_tipo, tipos_enum tipo)
{
    nodo_tipos_enum *nodo;
    nodo = malloc(sizeof(nodo_tipos_enum));

    nodo->tipo = tipo;
    nodo->prox = NULL; // ultimo elemento da pilha
    pilha_tipo->tam+=1;
    
    if(pilha_tipo->tam == 1)
        pilha_tipo->ultimo = nodo;
    else
    {
        nodo->prox = pilha_tipo->ultimo;
        pilha_tipo->ultimo = nodo;
    }    
}

int remove_elemento_tipo_pilha(pilha_tipo_enum *pilha_tipo)
{
    nodo_tipos_enum *nodo;

    nodo = pilha_tipo->ultimo;
    if( (nodo == NULL) || (pilha_tipo->tam == 0))
        error_handler("[ERRO] a pilha nao possui elementos para serem removidos\n");

    pilha_tipo->ultimo = pilha_tipo->ultimo->prox;
    pilha_tipo->tam-=1;
    free(nodo);
}

void destruir_pilha_tipos(pilha_tipo_enum *pilha_tipo)
{
    nodo_tipos_enum *nodo, *tmp;

    if(pilha_tipo->tam != 0)
        debug("[ERRO] A pilha de tipos tem tamanho: [%d]. Masdeveria estar vazia, em caso de execução perfeita\n",pilha_tipo->tam);     

    nodo = pilha_tipo->ultimo;

    while(nodo != NULL)
    {
        tmp = nodo;
        nodo = nodo->prox;
        free(tmp);
    }
    free(pilha_tipo);
    pilha_tipo = NULL;
}

// Funcoes da pilha para verificacao de atributo tipos
// nao entendi direito o funcionamento dessa
int verificador_tipos(pilha_tipo_enum *pilha_tipo, tipos_enum esperado, tipos_enum resultado)
{
    tipos_enum var1, var2;

    var1 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);

    var2 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);

    if ( (var1 == esperado) && ( var2 == esperado) )
        insere_elemento_tipo_pilha(pilha_tipo, resultado);
    else
        error_handler("Tipo de dados nao sao iguais.\n");
}

int verifica_atributo_tipos(pilha_tipo_enum *pilha_tipo)
{
    tipos_enum var1, var2;
    var1 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);

    var2 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);

    if (var1 != var2)
        error_handler("Tipos de dados nao sao iguais.\n");
}

int verifica_tipos_expressao(pilha_tipo_enum *pilha_tipo, tipos_enum esperado)
{
    tipos_enum var1;
    var1 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);
    if (var1 != esperado)
        error_handler("Tipos de dados nao sao iguais. \n");
}

void andar_pilha_tipos(pilha_tipo_enum *pilha_tipo) // parece ser func de debug
{
    nodo_tipos_enum *nodo;
    nodo = pilha_tipo->ultimo;

    for(int nivel = 0; (nodo != NULL); nivel++)
    {
        debug("[DEBUG]tipo = [%d], nivel = [%d]\n",nodo->tipo, nivel);
        nodo = nodo->prox;
        debug("[warning] All nodes from stack should have been consumed\n");
    }
}
/* -------------------- FIM FUNCOES DA PILHA DE TIPOS -------------------- */

/* -------------------- COMECO FUNCOES DA PILHA DE ROTULOS -------------------- */

char *insere_rotulo(pilha_rotulo *p_rotulo)
{
    nodo_rotulo *nodo;

    nodo = malloc(sizeof(nodo_rotulo));
    nodo->prox = NULL;
    // vai criando os rotulos sobre demanda
    sprintf(nodo->rotulo,"R%02d", p_rotulo->atual);
    p_rotulo->atual+=1;

    if (p_rotulo->atual == 1)
        p_rotulo->ultimo = nodo;
    else
    {
        nodo->prox = p_rotulo->ultimo;
        p_rotulo->ultimo = nodo;
    }

    return nodo->rotulo;
}

int remove_rotulo(pilha_rotulo *p_rotulo)
{
    nodo_rotulo *nodo;
    nodo = p_rotulo->ultimo;

    if ((nodo == NULL) || (p_rotulo->atual == 0))
        error_handler("Pilha de rotulos sem elemento. Erro de acesso.\n");
    
    p_rotulo->ultimo = p_rotulo->ultimo->prox;
    free(nodo);
}

void destroi_pilha_rotulo(pilha_rotulo *p_rotulo)
{
    nodo_rotulo *nodo, *tmp;

    nodo = p_rotulo->ultimo;
    while(nodo != NULL)
    {
        tmp = nodo;
        nodo = nodo->prox;
        free(tmp);
    }

    free(p_rotulo);
    p_rotulo = NULL;
}

void andar_pilha_rotulo(pilha_rotulo *p_rotulo) // para debug?
{
    nodo_rotulo *nodo;
    nodo = p_rotulo->ultimo;

    for(int nivel = 0; (nodo != NULL); nivel++)
    {
        debug("[DEBUG]tipo = [%s], nivel = [%d]\n",nodo->rotulo, nivel);
        nodo = nodo->prox;
        debug("[warning] All nodes from stack should have been consumed\n");
    }
}

/* -------------------- FIM FUNCOES DA PILHA DE ROTULOS -------------------- */

/* -------------------- COMECO FUNCOES DA PILHA DE DESLOCAMENTO -------------------- */

void pilha_insere_deslocamento(pilha_deslocamento *p_deslocamento, int quantidade)
{
    nodo_deslocamento *nodo;
    nodo = malloc(sizeof(nodo_deslocamento));
    nodo->prox = NULL;
    nodo->desloc = quantidade;

    p_deslocamento->nv_lexico+=1;

    if (p_deslocamento->nv_lexico == 1)
        p_deslocamento->ultimo = nodo;
    else
    {
        nodo->prox = p_deslocamento->ultimo;
        p_deslocamento->ultimo = nodo;
    }
}

int pilha_remove_deslocamento(pilha_deslocamento *p_deslocamento)
{
    nodo_deslocamento *nodo;
    nodo = p_deslocamento->ultimo;

    if( (nodo == NULL) || (p_deslocamento->nv_lexico == 0))
        error_handler("[ERRO] a pilha deslocamento nao possui elementos para serem removidos\n");

    p_deslocamento->ultimo = p_deslocamento->ultimo->prox;
    p_deslocamento->nv_lexico-=1;

    free(nodo);
}

void destroi_pilha_deslocamento(pilha_deslocamento *p_deslocamento)
{
    nodo_deslocamento *nodo, *tmp;

    nodo = p_deslocamento->ultimo;
    while(nodo != NULL)
    {
        tmp = nodo;
        nodo = nodo->prox;
        free(tmp);
    }

    free(p_deslocamento);
    p_deslocamento = NULL;
}

int andar_pilha_deslocamento(pilha_deslocamento *p_deslocamento) // debug
{
    nodo_deslocamento *nodo;
    nodo = p_deslocamento->ultimo;

    for(int nivel = 0; (nodo != NULL); nivel++)
    {
        debug("[DEBUG]tipo = [%s], nivel = [%d]\n",nodo->desloc, nivel);
        nodo = nodo->prox;
        debug("[warning] All nodes from stack should have been consumed\n");
    }
}

/* -------------------- FIM FUNCOES DA PILHA DE DESLOCAMENTO -------------------- */


/* -------------------- COMECO FUNCOES DA PILHA DE PROCEDIMENTO C/ ROTULO -------------------- */
char *insere_pilha_procedimento_rotulo(pilha_procedimento_rotulo *p_procrot)
{
    nodo_procedimento_rotulo *nodo;

    nodo = malloc(sizeof(nodo_procedimento_rotulo));
    nodo->prox = NULL;

    sprintf(nodo->rotulo,"P%02d", p_procrot->atual); // da um rotulo para o atual

    p_procrot->atual+=1;

    if (p_procrot->atual == 1)
        p_procrot->ultimo = nodo;
    else
    {
        nodo->prox = p_procrot->ultimo;
        p_procrot->ultimo = nodo;
    }

    return nodo->rotulo;
}

void destroi_pilha_procedimento_rotulo(pilha_procedimento_rotulo *p_procrot)
{
    nodo_procedimento_rotulo *nodo, *tmp;

    nodo = p_procrot->ultimo;
    while(nodo != NULL)
    {
        tmp = nodo;
        nodo = nodo->prox;
        free(tmp);
    }

    free(p_procrot);
    p_procrot = NULL;
}

/* -------------------- FIM FUNCOES DA PILHA DE PROCEDIMENTO C/ ROTULO -------------------- */

/* -------------------- COMECO FUNCOES DA PILHA DE PROCEDIMENTO C/ SIMBOLO -------------------- */

void insere_pilha_simb(pilha_proc_simb *p_proc_sb, tab_simbolo *simb)
{
    nodo_proc_simb *nodo;
    
    nodo = malloc(sizeof(nodo_procedimento_rotulo));
    nodo->prox = NULL;

    nodo->simbolo = simb;

    p_proc_sb->atual+=1;

    if (p_proc_sb->atual == 1)
        p_proc_sb->ultimo = nodo;
    else
    {
        nodo->prox = p_proc_sb->ultimo;
        p_proc_sb->ultimo = nodo;
    }

}

int remove_pilha_simb(pilha_proc_simb *p_proc_sb)
{
    nodo_proc_simb *nodo;
    nodo = p_proc_sb->ultimo;

    if( (nodo == NULL) || (p_proc_sb->atual == 0))
        error_handler("[ERRO] a pilha deslocamento nao possui elementos para serem removidos\n");

    p_proc_sb->ultimo = p_proc_sb->ultimo->prox;
    p_proc_sb->atual-=1;

    free(nodo);
}

void destroi_pilha_simb(pilha_proc_simb *p_proc_sb)
{
    nodo_proc_simb *nodo, *tmp;

    nodo = p_proc_sb->ultimo;
    while(nodo != NULL)
    {
        tmp = nodo;
        nodo = nodo->prox;
        free(tmp);
    }

    free(p_proc_sb);
    p_proc_sb = NULL;
}

/* -------------------- FIM FUNCOES DA PILHA DE PROCEDIMENTO C/ ROTULO -------------------- */