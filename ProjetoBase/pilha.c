/*
# PARA REMOVER DEPOIS: nao entendi direito o funcionamento dessa
*/

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
    nodo->tam+=1;
    
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
        error_handler("[ERRO] a pilha nao possui elementos para serem removidos");

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
        error_handler("Tipo de dados nao sao iguais.");
}

int verifica_atributo_tipos(pilha_tipo_enum *pilha_tipo)
{
    tipos_enum var1, var2;
    var1 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);

    var2 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);

    if (var1 != var2)
        error_handler("Tipos de dados nao sao iguais.");
}

int verifica_tipos_expressao(pilha_tipo_enum *pilha_tipo, tipos_enum esperado)
{
    tipos_enum var1;
    var1 = pilha_tipo->ultimo->tipo;
    remove_elemento_tipo_pilha(pilha_tipo);
    if (var1 != esperado)
        error_handler("Tipos de dados nao sao iguais.");
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