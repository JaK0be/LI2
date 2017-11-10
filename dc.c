#include <stdio.h>
#include "parser.h"

/**
@file dc.c
Ficheiro principal que contem a funçao main, vai gerar o header da pagina, tal como o footer, e vai invocar a parse para criar o tabuleiro de jogo
*/

/**
\brief Funçao que vai imprimir o header da pagina web
*/
void print_header (){
	printf ("content-type: text/html\n\n");
	printf ("<body>\n");
	printf ("<svg width=1500 height=1360>\n");
}

/**
\brief Funçao que vai imprimir o footer da pagina web
*/
void print_footer(){
	printf ("</svg>\n");
	printf ("</body>\n");
	
}

/**
\brief Funçao main que vai invocar todas as funçoes que vao criar o jogo
*/
int main (){
	print_header ();
	parse ();
	print_footer();
	return 0;
}
