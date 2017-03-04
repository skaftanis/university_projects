%{
#include <stdio.h>
#include "myscanner.h"
void yyerror(char *); 
extern int yylineno;
extern char* yytext;						
%}

%token INT0
%token INT
%left ':'
%%

program: expr 	{printf(yytext, "%i\n", $1);}
		;

expr: INT0 ":" INT 
	  { printf("nice"); }
	  ;


%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}


int main ( int argc, char **argv  ) 
  {
  ++argv; --argc;
  //if ( argc > 0 )
  		printf("%d\n", argv[0] );
 // else
    //    yyin = stdin;
  //yyout = fopen ( "output", "w" );	
  yyparse ();	    
  return 0;
  } 
