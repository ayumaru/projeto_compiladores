
// Testar se funciona corretamente o empilhamento de par�metros
// passados por valor ou por referencia.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "debug.h"
#include "funcoes.h"

tab_simbolo *aux, *tmp_att, *simbolo_procedimento, *simbolo_funcao;
tab_simbolo *simbolo_desvio, *contexto_atual;

pilha_proc_simb *pilha_simbolos_procedimentos;

pilha_tab_simbolo *tabela;

pilha_tipo_enum *pilha_tipos;
pilha_rotulo *p_rotulo;

tipos_enum element;


char *rot_atual, *rot_ant;

int num_vars, t_num_vars, proc_num_vars, var_local, par_var = 0;
int nv_lexico, deslocamento;
int fg = 0, pos_param = 0, soma_simp, atual_qnt_simb;
int fg_pass = 0, op_atrib = 0, pos_param_func = 0;
int salto[TAM_TOKEN], pos_salto = 0;

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token ABRE_COLCHETES FECHA_COLCHETES ABRE_CHAVES FECHA_CHAVES
%token MAIOR MAIOR_IGUAL MENOR MENOR_IGUAL IGUAL DIFERENTE
%token SOMA SUBTRACAO MULTIPLICACAO DIVISAO 
%token AND OR LABEL TYPE ARRAY OF PROCEDURE FUNCTION
%token IF ELSE WHILE DO NOT THEN
%token INTEGER BOOLEAN NUMBER
%token TRUE FALSE
%token READ WRITE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

programa:   {
               geraCodigo (NULL, "INPP");
               nv_lexico = 0;
               deslocamento = 0;
               t_num_vars = 0;
            }
            PROGRAM IDENT { aux = adicionar_simbolo(tabela, token, cat_PROGRAMA, 0, 0);  debug("[debug] BUCETA ****\n");  }
            ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
            bloco PONTO
            {
               if(t_num_vars)
               {
                  pp_geraCodigo(NULL, "DMEM %d", t_num_vars);
                  debug("[debug] BATATA **** desaloca memoria para [%d] variaveis \n",t_num_vars);
               }
               geraCodigo (NULL, "PARA");
            }
;

bloco:   parte_declara_vars
         {
            debug("[debug] LINE 76 ****\n");
            if ( fg == 0 )
            {
               debug("[debug] LINE 79 ****\n");
               rot_atual =  insere_rotulo(p_rotulo);
               pp_geraCodigo(NULL, "DSVS %s", rot_atual);
            }
            else if ( fg >= 1 )
            {
               debug("[debug] LINE 85 ****\n");simbolo_desvio = pilha_simbolos_procedimentos->ultimo->simbolo;
               if( simbolo_desvio->desvio == 0 )
               {
                  debug("[debug] LINE 88 ****\n");rot_atual = simbolo_desvio->rotulo_desv;
                  pp_geraCodigo(NULL, "DSVS %s", rot_atual);
                  simbolo_desvio->desvio+=1;
               }
            }
         }
         subrots comando_composto
;

parte_declara_vars:  var
;

// para ter varias subrotinas
subrots : subrots subrotinas 
         | subrotinas


subrotinas: {debug("[debug] LINE 105 ****\n"); } dec_proced
            | {debug("[debug] LINE 106 ****\n");} dec_func
            | {debug("[debug] LINE 107 ****\n");} // nada
;

dec_proced: PROCEDURE dec_proced_pre PONTO_E_VIRGULA dec_proced_pos
;

dec_func: FUNCTION dec_proced_pre DOIS_PONTOS def_tipo_func PONTO_E_VIRGULA { simbolo_procedimento->categoria = cat_FUNC; } dec_proced_pos
;

def_tipo_func: INTEGER { simbolo_procedimento->tipo = tipo_INT; }
             | BOOLEAN { simbolo_procedimento->tipo = tipo_BOOL; }


dec_proced_pre: IDENT
                  {
                     nv_lexico+=1;
                     fg+= 1;
                     simbolo_procedimento = adicionar_simbolo(tabela, token, cat_PROCED, nv_lexico, deslocamento);
                     contexto_atual = simbolo_procedimento;
                     rot_atual = insere_rotulo(p_rotulo);
                     pp_geraCodigo(rot_atual, "ENPR %d", nv_lexico);
                     insere_pilha_simb(pilha_simbolos_procedimentos, simbolo_procedimento);
                     strcpy(simbolo_procedimento->rotulo, rot_atual);
                     rot_atual = insere_rotulo(p_rotulo);
                     strcpy(simbolo_procedimento->rotulo_desv, rot_atual);
                     simbolo_procedimento->desvio = 0;
                     debug("[debug] Rotulo [%s], do simbolo [%s] definido com sucesso\n",simbolo_procedimento->id, simbolo_procedimento->rotulo);
                     proc_num_vars = 0;
                  }

                  parametros_formais
                  {
                     if ( proc_num_vars ) // 0 or -1
                     {
                        simbolo_procedimento->qnt_parametros = proc_num_vars;
                        atualiza_simbolo_procedimento_tabela_simbolo(tabela, -4, simbolo_procedimento);
                     }
                  }
;
;

dec_proced_pos: bloco
                  {
                     simbolo_procedimento = pilha_simbolos_procedimentos->ultimo->simbolo;
                     var_local = contagem_variaveis_locais(tabela, simbolo_procedimento);
                     t_num_vars = t_num_vars - var_local;

                     if (var_local) // != 0 or -1
                        pp_geraCodigo(NULL, "DMEM %d", var_local);
                     
                     pp_geraCodigo(NULL, "RTPR %d, %d", nv_lexico, simbolo_procedimento->qnt_parametros);
                     nv_lexico-=1;
                     remove_pilha_simb(pilha_simbolos_procedimentos);
                     simbolo_procedimento = NULL;
                     fg-=1;

                     if( pilha_simbolos_procedimentos->ultimo )
                        contexto_atual = pilha_simbolos_procedimentos->ultimo->simbolo;
                  }
               PONTO_E_VIRGULA            
;

parametros_formais: ABRE_PARENTESES declara_param_formais_list FECHA_PARENTESES 
                  | // faz nada com parenteses
;

declara_param_formais_list: declara_param_formais_list VIRGULA { num_vars = 0; } declara_param_formais
                          | declara_param_formais_list PONTO_E_VIRGULA { num_vars = 0; } declara_param_formais
                          | { num_vars = 0; } declara_param_formais
;

declara_param_formais: lista_id_var DOIS_PONTOS tipo { proc_num_vars+= num_vars; /* aumentar o num de vars em relacao ao procedimento*/ } 
;

var: VAR { deslocamento = 0; } declara_vars
   |
;

declara_vars: declara_vars { num_vars = 0; } declara_var
            | { num_vars = 0; } declara_var
;

declara_var:   lista_id_var DOIS_PONTOS
               tipo
               { 
                  t_num_vars+= num_vars;
                  par_var+= num_vars;
               }
               PONTO_E_VIRGULA
               {
                  pp_geraCodigo(NULL, "AMEM %d", par_var);
                  debug("aloca memoria para [%d] variavei[s]\n",par_var);
                  par_var = 0;
               }
;

lista_id_var: lista_id_var VIRGULA variaveis
            | variaveis
;

variaveis:  IDENT 
            {
               aux = adicionar_simbolo(tabela, token, cat_var_SIMPLES, nv_lexico, deslocamento);
               deslocamento+= 1;
               num_vars+= 1;
               aux->tipo_param = param_tipo_VALOR;
               debug("[%s] definido como [passagem por valor]\n",aux->id);
            }
            | VAR IDENT
            {
               aux = adicionar_simbolo(tabela, token, cat_var_SIMPLES, nv_lexico, deslocamento);
               deslocamento+= 1;
               num_vars+= 1;
               aux->tipo_param = param_tipo_REF;
               debug("[%s] definido como [passagem por referencia]\n",aux->id);
            }
;

tipo: INTEGER { atualiza_simbolo_tabela_tipo(tabela, tipo_INT); }
    | BOOLEAN { atualiza_simbolo_tabela_tipo(tabela, tipo_BOOL); }
;

lista_idents: lista_idents VIRGULA IDENT
            | IDENT
;

comando_composto: T_BEGIN 
                     {
                        debug("[debug] LINE 234 ****, fg: [%d] \n", fg);
                        if (fg == 0)
                        {
                           fg = -1;
                           geraCodigo("R00", "NADA");
                        }
                        else if (fg >= 1 )
                        {
                           debug("[debug] LINE 242 ****\n");simbolo_desvio = pilha_simbolos_procedimentos->ultimo->simbolo;
                           if (simbolo_desvio->desvio == 1)
                           {
                              debug("[debug] LINE 245 ****\n");rot_atual = simbolo_desvio->rotulo_desv;
                              geraCodigo(rot_atual, "NADA");
                              simbolo_desvio->desvio+= 1;
                           }
                        }
                     } lista_comands T_END
                | //faz nada
;

lista_comands: lista_comands PONTO_E_VIRGULA {debug("[debug] LINE 254 pos ****\n");}comandos
             | {debug("[debug] LINE 255 ****\n");}comandos
;

comandos: {debug("[debug] LINE 258 ****\n");} comando_srotulo
;

comando_srotulo: {debug("[debug] LINE 261 ****\n");} leitura_identificadores {debug("[debug] LINE 261 pos ****\n");}
               | {debug("[debug] LINE 262 ****\n");}comando_composto
               | {debug("[debug] LINE 263 ****\n");}comando_repeticao
               | {debug("[debug] LINE 264 ****\n");}leitura_escrita
               | {debug("[debug] LINE 265 ****\n");}condicional_if
;

leitura_escrita: READ ABRE_PARENTESES lista_parametros_leitura FECHA_PARENTESES
               | WRITE ABRE_PARENTESES lista_parametros_escrita FECHA_PARENTESES
;

lista_parametros_leitura: lista_parametros_leitura VIRGULA leitor_parametros
                        | leitor_parametros
;

leitor_parametros: IDENT
                     {
                        debug("variavel token [read] idenfiticado = [%s]\n",token);
                        aux = busca_simbolo(tabela, token); // pega o simbolo da tabela, gera codigo
                        geraCodigo(NULL, "LEIT");
                        pp_geraCodigo(NULL, "ARMZ %d, %d", aux->nv_lexico, aux->deslocamento);
                     }
;

lista_parametros_escrita: lista_parametros_escrita VIRGULA escritor_parametros
                        | escritor_parametros
;

escritor_parametros: leitor_variaveis_escrita 
                     { 
                        remove_elemento_tipo_pilha(pilha_tipos);
                        geraCodigo(NULL, "IMPR");
                     }
                   | NUMBER { debug("[write] numero identificado =  [%d]\n",atoi(token)); pp_geraCodigo(NULL, "CRCT %d", atoi(token)); geraCodigo(NULL, "IMPR"); }
;

leitor_variaveis_escrita:  IDENT { aux =  busca_simbolo(tabela, token); } eh_funcao
;

pre_if: IF expressao
         {
            verifica_tipos_expressao(pilha_tipos, tipo_BOOL);
            rot_atual = insere_rotulo(p_rotulo); //R02 e desvia 
            pp_geraCodigo(NULL, "DSVF %s", rot_atual);
         } THEN comando_srotulo
;

condicional_if: pre_if %prec LOWER_THAN_ELSE
                  {
                     rot_atual = p_rotulo->ultimo->rotulo;
                     debug("[debug/////] Rotulo do then sem else [%s]\n",rot_atual);
                     geraCodigo(rot_atual, "NADA");
                     remove_rotulo(p_rotulo);
                  }
              | pre_if ELSE 
                  {
                     rot_atual = insere_rotulo(p_rotulo);
                     //rot_atual = p_rotulo->ultimo->rotulo;
                     rot_ant = p_rotulo->ultimo->prox->rotulo;
                     pp_geraCodigo(NULL, "DSVS %s", rot_atual);
                     geraCodigo(rot_ant, "NADA");
                  }
                comando_srotulo
                  {
                     rot_atual = p_rotulo->ultimo->rotulo;
                     geraCodigo(rot_atual, "NADA");
                     remove_rotulo(p_rotulo);
                     remove_rotulo(p_rotulo);
                  } 
;

comando_repeticao:   WHILE 
                     {
                        rot_atual = insere_rotulo(p_rotulo);
                        geraCodigo(rot_atual, "NADA");
                     }
                     expressao
                     {
                        rot_atual = insere_rotulo(p_rotulo);
                        pp_geraCodigo(NULL, "DSVF %s", rot_atual);
                        verifica_tipos_expressao(pilha_tipos, tipo_BOOL);
                     }
                     DO comando_srotulo 
                     {
                        // R00 ->  rot anterior, R01 -> rot atual
                        rot_atual = p_rotulo->ultimo->rotulo;
                        rot_ant = p_rotulo->ultimo->prox->rotulo;
                        pp_geraCodigo(NULL, "DSVS %s", rot_ant);
                        geraCodigo(rot_atual, "NADA");
                        remove_rotulo(p_rotulo);
                        remove_rotulo(p_rotulo);
                     }
;

leitura_identificadores: IDENT { debug("[debug] LINE 353 **** token: [%s] || tabela: %s \n", token, tabela->ultimo->id); tmp_att = busca_simbolo(tabela, token); } atribuicao_execucao


atribuicao_execucao: {debug("[debug] LINE 356 ****\n");}atribuicao_simples
                   | {debug("[debug] LINE 357 ****\n");}executa
;

atribuicao_simples: ATRIBUICAO expressao
                     {
                        debug("[debug] LINE 362 ****\n");if ( tmp_att->categoria  == cat_PROCED )
                           error_handler("Procedimentos nao podem ser utilizados como atribuicao");
                        
                        element = tmp_att->tipo;
                        insere_elemento_tipo_pilha(pilha_tipos, element);
                        verifica_atributo_tipos(pilha_tipos);
                        
                        if ( tmp_att->tipo_param == param_tipo_REF )
                        {
                           pp_geraCodigo(NULL, "ARMI %d, %d", tmp_att->nv_lexico, tmp_att->deslocamento);
                        }
                        else
                        {
                           if (  tmp_att->categoria == cat_FUNC )
                           {
                              if ( contexto_atual != tmp_att )
                                 error_handler("A funcao nao pode ser utilizada para atribuicao quando nao esta dentro de seu contexto");
                           }
                           pp_geraCodigo(NULL, "ARMZ %d, %d", tmp_att->nv_lexico, tmp_att->deslocamento);
                        }
                     }
;

executa: leitura_vars_procedimento { pp_geraCodigo(NULL, "CHPR %s, %d", tmp_att->rotulo, nv_lexico); }


leitura_vars_procedimento: ABRE_PARENTESES FECHA_COLCHETES // nao teve vars
                         | ABRE_PARENTESES { atual_qnt_simb = tmp_att->qnt_parametros; pos_param = 0; } params_procedimento FECHA_PARENTESES
                         | // faz nada


params_procedimento: params_procedimento VIRGULA testagem
                   | testagem
;

testagem: { 
            op_atrib = 0;
            if ( tmp_att->list_passagem_tipo[pos_param] == param_tipo_REF )
               fg_pass = 1;
          }
         expressao
         {
            element = tmp_att->list_procedimentos_tipo[pos_param];
            insere_elemento_tipo_pilha(pilha_tipos, element);
            soma_simp = atual_qnt_simb * -1 + -3 + pos_param;
            verifica_atributo_tipos(pilha_tipos);
            pos_param+= 1;
            fg_pass = 0;
          }
;

expressao: operacao MAIOR operacao {  verificador_tipos(pilha_tipos, tipo_INT, tipo_BOOL); geraCodigo(NULL, "CMMA");}
         | operacao MAIOR_IGUAL operacao {  verificador_tipos(pilha_tipos, tipo_INT, tipo_BOOL); geraCodigo(NULL, "CMAG");}
         | operacao MENOR operacao { debug("[debug] LINE 415 pre verificador ****\n"); verificador_tipos(pilha_tipos, tipo_INT, tipo_BOOL); geraCodigo(NULL, "CMME");}
         | operacao MENOR_IGUAL operacao {  verificador_tipos(pilha_tipos, tipo_INT, tipo_BOOL); geraCodigo(NULL, "CMEG");}
         | operacao IGUAL operacao {  verificador_tipos(pilha_tipos, tipo_INT, tipo_BOOL); geraCodigo(NULL, "CMIG");}
         | operacao DIFERENTE operacao {  verificador_tipos(pilha_tipos, tipo_INT, tipo_BOOL); geraCodigo(NULL, "CMDG");}
         | operacao {debug("[debug] LINE 419 ****\n");}
;

operacao: operacao SOMA termo { verificador_tipos(pilha_tipos, tipo_INT, tipo_INT); geraCodigo(NULL, "SOMA"); }
        | operacao SUBTRACAO termo { verificador_tipos(pilha_tipos, tipo_INT, tipo_INT); geraCodigo(NULL, "SUBT"); }
        | operacao OR termo { verificador_tipos(pilha_tipos, tipo_BOOL, tipo_BOOL); geraCodigo(NULL, "DISJ"); }
        | termo {debug("[debug] LINE 425 ****\n");}
;

termo: termo MULTIPLICACAO pre_fator { verificador_tipos(pilha_tipos, tipo_INT, tipo_INT); geraCodigo(NULL, "MULT"); }
     | termo DIVISAO pre_fator { verificador_tipos(pilha_tipos, tipo_INT, tipo_INT); geraCodigo(NULL, "DIVI"); }
     | termo AND pre_fator { verificador_tipos(pilha_tipos, tipo_BOOL, tipo_BOOL); geraCodigo(NULL, "CONJ"); }
     | {debug("[debug] LINE 431 ****\n");}pre_fator {debug("[debug] LINE 431 pos ****\n");}
;

pre_fator:  {
               debug("[debug] LINE 435 ****\n"); op_atrib+= 1;
               if (fg_pass && (op_atrib > 1))
               {
                  debug("[debug] LINE 438 ****\n");if (tmp_att)
                     error_handler("Nao eh permitido atribuicao em passagem por referencia");
                  else
                     error_handler("Nao eh permitido atribuicao em passagem por referencia");
               }
            } fator
;

fator: leitura_variaveis 
     | NUMBER { pp_geraCodigo(NULL, "CRCT %d", atoi(token)); insere_elemento_tipo_pilha(pilha_tipos, tipo_INT); }
     | TRUE { geraCodigo(NULL, "CRCT 1"); insere_elemento_tipo_pilha(pilha_tipos, tipo_BOOL); }
     | FALSE { geraCodigo(NULL, "CRCT 0"); insere_elemento_tipo_pilha(pilha_tipos, tipo_BOOL); }
     | {debug("[debug] LINE 450 pre ****\n");} ABRE_PARENTESES expressao {debug("[debug] LINE 450 pos ****\n");}FECHA_PARENTESES
;

leitura_variaveis: IDENT { debug("[debug] LINE 453 pre ****\n");aux = busca_simbolo(tabela, token); } eh_funcao
;

eh_funcao: ABRE_PARENTESES { geraCodigo(NULL, "AMEM 1"); } declara_func
         |  { 
               if (fg_pass) //!= 0 or -1
               {
                  if ( aux->tipo_param == param_tipo_REF )
                  {
                       pp_geraCodigo(NULL, "CRVL %d, %d", aux->nv_lexico, aux->deslocamento);
                  }
                  else
                  {
                     pp_geraCodigo(NULL, "CREN %d, %d", aux->nv_lexico, aux->deslocamento);
                  }
               }
               else if ( aux->tipo_param == param_tipo_REF )
               {
                  pp_geraCodigo(NULL, "CRVI %d, %d", aux->nv_lexico, aux->deslocamento);
               }
               else
               {
                     pp_geraCodigo(NULL, "CRVL %d, %d", aux->nv_lexico, aux->deslocamento);
               }
               insere_elemento_tipo_pilha(pilha_tipos, aux->tipo);
            }


declara_func: FECHA_PARENTESES {debug("[debug] LINE 479 pre ****\n"); insere_elemento_tipo_pilha(pilha_tipos, aux->tipo); }
            | declara_params_funcao FECHA_PARENTESES { insere_elemento_tipo_pilha(pilha_tipos, simbolo_funcao->tipo); }
;

declara_params_funcao:  { simbolo_funcao = aux; } leitura_vars_funcao 
                        {
                           pp_geraCodigo(NULL, "CHPR %s, %d", simbolo_funcao->rotulo, nv_lexico);
                           if (pos_salto) //!= 0 ir -1
                           {
                              pos_param-= 1;
                              pos_param_func = salto[pos_param];
                           }
                        }
;

leitura_vars_funcao: {
                        if (pos_param)
                           salto[pos_param] = pos_param_func;
                        
                        atual_qnt_simb = simbolo_funcao->qnt_parametros;
                        pos_param_func = 0;
                        pos_param+= 1;
                     } parametros_da_funcao
;

parametros_da_funcao: parametros_da_funcao VIRGULA parametro_da_funcao
                    | parametro_da_funcao
;

parametro_da_funcao: {
                        op_atrib = 0;
                        if ( simbolo_funcao->list_passagem_tipo[pos_param_func] ==  param_tipo_REF) 
                           fg_pass = 1;
                     }
                     expressao 
                     {
                        element = simbolo_funcao->list_procedimentos_tipo[pos_param_func];
                        insere_elemento_tipo_pilha(pilha_tipos, element);
                        verifica_atributo_tipos(pilha_tipos);
                        soma_simp = atual_qnt_simb * -1 + -3 + pos_param_func;
                        pos_param_func+= 1;
                        fg_pass = 0;
                     }
;

%%

int error_handler(char *s)
{
   desaloca();  
   imprimeErro(s);  
}

int desaloca()
{
   deletar_tabela_simbolo(tabela);
   destruir_pilha_tipos(pilha_tipos);
   destroi_pilha_rotulo(p_rotulo);
   destroi_pilha_simb(pilha_simbolos_procedimentos);
}


int main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }

   tabela = malloc(sizeof(pilha_tab_simbolo));
   tabela->primeiro = NULL;
   tabela->ultimo = NULL;
   tabela->tam = 0;

   pilha_tipos = malloc(sizeof(pilha_tipo_enum));
   pilha_tipos->ultimo = NULL;
   pilha_tipos->tam = 0;

   p_rotulo = malloc(sizeof(pilha_rotulo));
   p_rotulo->ultimo = NULL;
   p_rotulo->atual = 0;

   pilha_simbolos_procedimentos = malloc(sizeof(pilha_proc_simb));
   pilha_simbolos_procedimentos->ultimo = NULL;
   pilha_simbolos_procedimentos->atual = 0;

/* -------------------------------------------------------------------
 *  Inicia a Tabela de Simbolos
 * ------------------------------------------------------------------- */

   yyin=fp;
   yyparse();

   imprime_tabela_simbolo(tabela);
   desaloca();
   return 0;
}
