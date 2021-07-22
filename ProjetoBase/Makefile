 # -------------------------------------------------------------------
 #            Arquivo: Makefile
 # -------------------------------------------------------------------
 #              Autor: Bruno Müller Junior
 #               Data: 08/2007
 #      Atualizado em: [09/08/2020, 19h:01m]
 #
 # -------------------------------------------------------------------

$DEPURA=1


compilador: lex.yy.c y.tab.c compilador.o funcoes.o pilha.o compilador.h funcoes.h pilha.h
	gcc lex.yy.c compilador.tab.c  compilador.o funcoes.o pilha.o -o compilador -ll -ly -lc

lex.yy.c: compilador.l compilador.h
	flex compilador.l

y.tab.c: compilador.y compilador.h
	bison compilador.y -d -v

compilador.o : compilador.h compiladorF.c
	gcc -c compiladorF.c -o compilador.o

pilha.o: pilha.c pilha.h
	gcc -c pilha.c -o pilha.o

funcoes.o: funcoes.c funcoes.h
	gcc -c funcoes.c -o funcoes.o

clean :
	rm -f compilador.tab.* lex.yy.c compilador.o compilador funcoes.o pilha.o
