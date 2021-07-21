#ifndef __FUNCOES__
#define __FUNCOES__

#define TAM_TOKEN 16
#define TAM_ROTULO 4

typedef enum categorias_enum {
    cat_var_SIMPLES = 1, cat_PROGRAMA, cat_param_FORMAL, cat_PROCED,  cat_FUNC
} categorias_enum;

typedef enum tipos_enum {
    tipo_INDEFINIDO = 0, tipo_INT, tipo_BOOL
} tipos_enum;

typedef enum tipo_passagem_param {
    param_tipo_INDEFINIDO, param_tipo_VALOR, param_tipo_REF
} tipo_passagem_param;

typedef struct tab_simbolo{
    char id[TAM_TOKEN];
    char rotulo[TAM_ROTULO];
    char rotulo_desv[TAM_ROTULO];
    char novo_tipo[TAM_TOKEN];
    char lista_novo_tipo[TAM_TOKEN][TAM_TOKEN];
    tipos_enum list_procedimentos_tipo[TAM_TOKEN];
    tipo_passagem_param list_passagem_tipo[TAM_TOKEN];
    tipos_enum tipo;
    categorias_enum categoria;
    int qnt_paramentros;
    int deslocamento;
    int nv_lexico;
    int desvio;
    tipo_passagem_param tipo_param;
    struct tab_simbolo *ant, *prox;
} tab_simbolo;

typedef struct pilha_tab_simbolo{
    struct tab_simbolo *primeiro, *ultimo;
    int tam;
} pilha_tab_simbolo;

// adicionar simbolos e buscar por eles
tab_simbolo *adicionar_simbolo(pilha_tab_simbolo *tabela, char *nome, tipos_enum categoria, int nv_lexico, int deslocamento);
tab_simbolo *busca_simbolo(pilha_tab_simbolo  *, char *);


// funcoes de auxilio para simbolos
void adicionar_tipo_simb(tab_simbolo *elemento_tab, tipos_enum tipo);
void remove_simbolo_tabela(pilha_tab_simbolo *, tab_simbolo *); 

int atualiza_simbolo_tabela_tipo(pilha_tab_simbolo *tabela, categorias_enum tipo);
int atualiza_simbolo_tabela_novo_tipo(pilha_tab_simbolo *tabela, categorias_enum tipo, char *nome);

void deletar_tabela_simbolo(pilha_tab_simbolo *tabela);
void deletar_tabela_xy_simbolos(pilha_tab_simbolo *tabela, int quantidade);

void atualiza_deslocament_tabela_simbolo(pilha_tab_simbolo *tabela, int deslocamento);
void atualiza_simbolo_procedimento_tabela_simbolo(pilha_tab_simbolo *tabela, int deslocamento, tab_simbolo *elemento_pai);

tab_simbolo *pega_rotulo_tabela_simbolo(pilha_tab_simbolo *tabela, char *rotulo);

int contagem_variaveis_locais(pilha_tab_simbolo *tabela, tab_simbolo *elemento_tab);

tipos_enum encontra_tipo_en_do_simbolo_procedimento(tab_simbolo *elemento_tab, int posicao);

tab_simbolo *busca_parametro_lista_simbolo(pilha_tab_simbolo *tabela, tab_simbolo *elemento_tab, int posicao);

void imprime_tabela_simbolo(pilha_tab_simbolo *tabela); // desnecessaria, vou deixar implementado e depois apago

#endif