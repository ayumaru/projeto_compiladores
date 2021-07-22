/*
TODO: Incluir as funcoes de debug
PARA BUSCA: termo >>>> nao sei pq faz isso, investigar
    - Verificar o motivo do pq foi feito isso
APAGAR: RAFAEL v
*/
#include "debug.h"
#include "funcoes.h"
#include "compilador.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


tab_simbolo *adicionar_simbolo(pilha_tab_simbolo *tabela, char *nome, tipos_enum categoria, int nv_lexico, int deslocamento)
{
    tab_simbolo *elemento_tab;
    elemento_tab = malloc(sizeof(tab_simbolo));

    strcpy(elemento_tab->id, nome);
    elemento_tab->categoria = categoria;
    elemento_tab->deslocamento = deslocamento;
    elemento_tab->nv_lexico = nv_lexico;
    
    elemento_tab->tipo = tipo_INDEFINIDO;
    elemento_tab->tipo_param = param_tipo_INDEFINIDO;
    // adicionando null para evitar lixo na string
    elemento_tab->rotulo[0] = '\0'; 
    elemento_tab->rotulo_desv[0] = '\0';
    elemento_tab->qnt_paramentros = 0; // comecando agora, nao e pra ter nada
    
    tabela->tam++;

    if (tabela->tam == 1)
    {
        tabela->primeiro = elemento_tab;
        elemento_tab->ant = NULL;
        elemento_tab->prox = NULL;
    }
    else
    {
        elemento_tab->ant = tabela->ultimo;
        elemento_tab->ant->prox = elemento_tab;
        elemento_tab->prox = NULL;  
    }

    tabela->ultimo = elemento_tab;

    // debugs

    return elemento_tab;
}

tab_simbolo *busca_simbolo(pilha_tab_simbolo *tabela, char *nome)
{
    tab_simbolo *elemento_tab;
    elemento_tab = tabela->ultimo;
    while (elemento_tab != NULL)
    {
        if ( strcmp(elemento_tab->id, nome)  == 0 )
        {
            // debugs 
            return elemento_tab;
        }

        elemento_tab = elemento_tab->ant;
    }
    
    debug("[ERRO] não foi possivel encontrar o simbolo [%s] na tabela. \n", nome);
    error_handler("Simbolo nao encontrado na tabela"); 
    return NULL;
}

void remove_simbolo_tabela(pilha_tab_simbolo *tabela, tab_simbolo *elemento_tab)
{
    // debug
    if( elemento_tab == tabela->primeiro )
        tabela->primeiro = elemento_tab->prox;
    else 
        elemento_tab->ant->prox = elemento_tab->prox;
    
    if ( elemento_tab == tabela->ultimo )
        tabela->ultimo = elemento_tab->ant;
    else
        elemento_tab->prox->ant = elemento_tab->ant;

    tabela->tam-=1;

    free(elemento_tab);
    // debug
}

int atualiza_simbolo_tabela_tipo(pilha_tab_simbolo *tabela, categorias_enum tipo)
{
    tab_simbolo *elemento_tab;

    elemento_tab = tabela->ultimo;
    while (  (elemento_tab != NULL) && (elemento_tab->tipo == tipo_INDEFINIDO)  )
    {
        elemento_tab->tipo = tipo;
        //debug
        elemento_tab = elemento_tab->ant;
    }
    
    return 0;
}


void deletar_tabela_simbolo(pilha_tab_simbolo *tabela)
{
    tab_simbolo *elemento_tab, *apagar;
    
    elemento_tab = tabela->primeiro;
    while ( elemento_tab != NULL  )
    {
        apagar = elemento_tab;
        elemento_tab = elemento_tab->prox;
        free(apagar);
    }
    
    free(tabela);
    tabela = NULL;
    //debug    
}

void deletar_tabela_xy_simbolos(pilha_tab_simbolo *tabela, int quantidade)
{
    tab_simbolo *elemento_tab, *apagar;
    int i = 0; 
    elemento_tab = tabela->ultimo;

    while ( (elemento_tab != NULL) ||  i <= quantidade )
    {
        apagar = elemento_tab;
        elemento_tab = elemento_tab->ant;
        free(apagar);
        i+=1;
    }
}

void atualiza_deslocament_tabela_simbolo(pilha_tab_simbolo *tabela, int deslocamento)
{
    tab_simbolo *elemento_tab;

    elemento_tab = tabela->ultimo;
    while ( elemento_tab->categoria == cat_param_FORMAL )
    {
        elemento_tab->deslocamento = deslocamento;
        //debug
        elemento_tab = elemento_tab->ant;
        deslocamento-=1; 
    }
}


/*
# Serve para modificar as variaveis simples para categoria de parametro formal
# Adiciona os tipos de valores ao vetor de tipo
*/
int atualiza_simbolo_procedimento_tabela_simbolo(pilha_tab_simbolo *tabela, int deslocamento, tab_simbolo *elemento_pai)
{
    tab_simbolo *elemento_tab;
    int posicao = elemento_pai->qnt_paramentros;

    posicao-=1;

    if (posicao > TAM_TOKEN)
        debug("[ERRO] O valor padrao de paramentros eh [16] \n");

    elemento_tab = tabela->ultimo;

    // rafael fez com somente 1 & comercial
    while( (elemento_tab->categoria != cat_PROCED) && (elemento_tab->categoria != cat_FUNC) )
    {
        elemento_tab->deslocamento = deslocamento;
        elemento_tab->categoria = cat_param_FORMAL;
        elemento_pai->list_procedimentos_tipo[posicao] = elemento_tab->tipo;
        elemento_pai->list_passagem_tipo[posicao] = elemento_tab->tipo_param;
        // debugs

        elemento_tab = elemento_tab->ant;
        deslocamento-=1;
        posicao-=1;
    }

    // truque para adicionar o deslocamento no simbolo de funcao na funcao
    elemento_tab->deslocamento = deslocamento;
    // debug
    if (elemento_tab != elemento_pai)
        debug("[ERRO] Simbolo deveria ser o primeiro a ser encontrado, entretanto nao foi [%s] != [%s] \n", elemento_tab->id, elemento_pai->id);

    return 0;
}

// devolve simbolo associado com o rotulo em questao
tab_simbolo *pega_rotulo_tabela_simbolo(pilha_tab_simbolo *tabela, char *rotulo)
{
    tab_simbolo *elemento_tab;

    //do ultimo para primeiro, pois vai definir os tipos indefinidos
    elemento_tab = tabela->ultimo;
    while (elemento_tab != NULL)
    {
        if ( elemento_tab->categoria == cat_PROCED )
        {
            if ( strcmp(elemento_tab->rotulo,rotulo) == 0 )
            {
                //debug
                return elemento_tab;
            }
        }

        elemento_tab = elemento_tab->ant;
    }
    
    //debug
    debug("[ERRO] Não existe variavel associada ao rotulo >> [%s]\n",rotulo);
    error_handler("Rotulo nao esta presente na tabela de simbolos.\n");
    return NULL; // nao achou 
}

/*
# Conveniencia de evitar acessar mais de uma vez a lista
# 1 Conta o numero da funcao
# 1 remove os simbolos que estao em nivel lexico maior que a funcao/procedimento atual
*/
int contagem_variaveis_locais(pilha_tab_simbolo *tabela, tab_simbolo *elemento_tab)
{
    int contador = 0;
    tab_simbolo *nodo, *aux;
    nodo = tabela->ultimo;

    while(nodo != elemento_tab)
        nodo = nodo->ant;
    
    if (nodo == NULL)
        error_handler("Erro. Não foi possivel encontrar o simbolo, e falha ao contar a quantidade de variaveis locais\n");
    // else debug sucesso

    nodo = nodo->prox;
    // if (nodo == NULL)
    //     error_handler("Erro. Não foi possivel encontrar simbolos apos procedimento ");
    //debug
    // RAFAEL v
    // A partir da aula 11, verifiquei a necessidade de excluir o parametro formal, porém quem baixa a pilha não é a instrução
    // Apesar de que faz sentido, visto que outros procedimentos podem declarar variaveis de mesmo nome, e não acontecera colisão
    // Os valores do tipo se encontra em list_procedimentos_tipo
    while( nodo && (nodo->nv_lexico == elemento_tab->nv_lexico) &&  (nodo->categoria == cat_param_FORMAL) )
    {
        // debug
        aux = nodo;
        nodo = nodo->prox;
        remove_simbolo_tabela(tabela, aux);
    }

    while( nodo && (nodo->nv_lexico == elemento_tab->nv_lexico) &&  (nodo->categoria == cat_var_SIMPLES) )
    {
        aux = nodo;
        nodo = nodo->prox;
        remove_simbolo_tabela(tabela, aux); // remove var da tabela, pois esta saido do procedimento
        contador+=1;
    }

    // agora vai ate o final da tabela
    while(nodo != NULL)
    {
        if ( nodo->nv_lexico == (elemento_tab->nv_lexico+1)  )
        {
            aux = nodo;
            nodo = nodo->prox;
            remove_simbolo_tabela(tabela, aux);
        }
        else
            nodo = nodo->prox;
    }

    //procura outras variaveis e delete elas
    return contador;
}

tipos_enum encontra_tipo_en_do_simbolo_procedimento(tab_simbolo *elemento_tab, int posicao)
{
    tab_simbolo *nodo;

    // talvez nao precise desses debug, mas vemos depois
    if (posicao > elemento_tab->qnt_paramentros )
    {
        debug("[ERRO] O procedimento [%s] não pode acessar a variavel, somente acessa [%d] variaveis, posicao ->[%d]",elemento_tab->id, elemento_tab->qnt_paramentros, posicao);
    }

    if (posicao < 0)
    {
        debug("[ERRO] Como o procedimento le de tras para frente sera acessado posicao - num vair, sendo assim possivel ultrapassar o numero de variaveis e passando um numero negativo");
        debug("[ERRO] O procedimento [%s] não pode acessar a variavel, somente acessa [%d] variaveis, posicao ->[%d]",elemento_tab->id, elemento_tab->qnt_paramentros, posicao);
    }

    nodo = elemento_tab->prox;

    while( (nodo != NULL) && (posicao != 0 ) )
    {
        posicao-=1;
        nodo = nodo->prox;
    }

    if ( posicao > 0 )
        debug("[ERRO] O nodo foi encontrado antes do tipo, verificar erro [%s], posicao ->[%d]", elemento_tab->id ,posicao);

    return nodo->tipo;
}

void imprime_tabela_simbolo(pilha_tab_simbolo *tabela) // apagar eventualmente 
{
    tab_simbolo *elemento_tab;
    // debug
    elemento_tab = tabela->ultimo;
    while (elemento_tab != NULL)
    {
        // debug
        elemento_tab = elemento_tab->ant;
    }
}