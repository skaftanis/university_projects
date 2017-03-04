#include <stdio.h>
#include "myscanner.h"


extern int yylex();
extern int yylineno;
extern char* yytext;

char *names[] = {NULL, "request_type", "Connection", "Date", "Transfer_encoding", "Request_Headers", "Entity_Header", "Message_Body" };

int main() {

	int ntoken, vtoken;

	int tokenCounter=1;
	ntoken = yylex();

	while (ntoken) {
		if (tokenCounter == 3 && ntoken != 11)  //έλεγχος για GET,HEAD,POST
				printf("Invalid token. Expected GET,HEAD or POST\n");
		else
				printf("%d\n",ntoken);
		ntoken = yylex();
		tokenCounter++;
	}

	return 0; 

}