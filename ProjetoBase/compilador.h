/* -------------------------------------------------------------------
 *            Arquivo: compilador.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [09/08/2020, 19h:01m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, prototipos e variaveis globais do compilador (via extern)
 *
 * ------------------------------------------------------------------- */
#ifndef __COMPILADOR__
#define __COMPILADOR__

#define TAM_TOKEN 16

typedef enum simbolos {
  simb_program, simb_var, simb_begin, simb_end,
  simb_identificador, simb_numero,
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses,
  simb_abre_colchetes, simb_fecha_colchetes, simb_abre_chaves, simb_fecha_chaves,
  simb_maior_que, simb_maior_igual, simb_menor_que, simb_menor_igual,
  simb_igual, simb_diferente, simb_soma, simb_subtracao, simb_multiplicacao,
  simb_divisao, simb_and, simb_or, simb_label, simb_type, simb_array,
  simb_of, simb_procedure, simb_function, simb_if, simb_else,
  simb_while, simb_do, simb_not, simb_then
} simbolos;



/* -------------------------------------------------------------------
 * variaveis globais
 * ------------------------------------------------------------------- */

extern simbolos simbolo, relacao;
extern char token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;
extern int nl;


extern simbolos simbolo, relacao;
extern char token[TAM_TOKEN];

/* -------------------------------------------------------------------
 * prototipos globais
 * ------------------------------------------------------------------- */
int imprimeErro(char*);
int error_handler(char *);
int desaloca();

void geraCodigo (char*, char*);
int yylex();
void yyerror(const char *s);

#endif