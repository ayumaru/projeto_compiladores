#include <stdio.h>
#include "compilador.h"

typedef struct simbolo{
    int nivel_lexico[2]; // nivel e deslocamento
    int categoria; // variavel simples, procedimento, etc
    char *identificador; // label
    int tipos; // tipos de variavel
    char nome[TAM_TOKEN];
} simbolo;

typedef struct{
    void *elemento;
    struct nodo *prox, *prev;
}nodo;

typedef struct{
    nodo *inicio, *fim;
    int tam;
}pilha;

void inicia_pilha(pilha *stack){
    stack->inicio = NULL;
    stack->fim = NULL;
    stack->tam = 0;
}

void verifica_vazio(pilha *stack){
    return (stack->inicio == NULL);
}

void insere_elemento(void *item, pilha *stack){ //alterar para pegar o fim
    nodo *novo = (nodo*)malloc(sizeof(nodo));
    novo->elemento = item;
    novo->prox = NULL;
    novo->prev = stack->fim;
    
    if (stack->fim == NULL) // rever isso
        stack->inicio = novo;
    else
        stack->fim->prev = novo;

    stack->fim = novo;
    stack->tam += 1;
}

void *retira_elemento(pilha *stack){
    nodo *aux;
    if ( verifica_vazio(stack) )
        return NULL;
    else{
        aux = stack->fim;
        void *elemento = aux->elemento;
        stack->fim = aux->prev;
        if (stack->fim != NULL)
            stack->fim->prev = NULL;
        else
            stack->inicio = NULL;

        free(aux);
        stack->tam -=1;
        return elemento;
    }
}

simbolo *busca_simbolo(char nome_simbolo[TAM_TOKEN], pilha *stack){
    nodo *temp;
    for ( temp =stack->fim; temp !=NULL; temp=temp->prev ) // busca de traz pra frente para respeitar o nivel 
    {
        simbolo *simb = (simbolo*)temp->elemento;
        if ( strcmp(simb->nome, nome_simbolo) == 0)
            return simb;
    } 
    return NULL; //nao achou o simbolo
}