./a.out : main.o token.o html.o type.o
	gcc main.o token.o html.o type.o
main.o : main.c
	gcc -c main.c
token.o : token.c
	gcc -c token.c		
html.o : html.c
	gcc -c html.c	
type.o : type.c
	gcc -c type.c