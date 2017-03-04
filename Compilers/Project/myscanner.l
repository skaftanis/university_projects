%{
#include "myscanner.h"
#include "y.tab.h"  
%}

%%
:				   	     return COLON;
"request_type"			 return INT0;
"general_header"		 return GENERAL_HEADER;
"Request_Headers"        return REQUEST_HEADERS;
"Entity_Header"		     return ENTITY_HEADER;
"Message_Body"		     return MESSAGE_BODY;



"GET"|"POST"|"HEAD"  {  return INT; }
"YES"|"NO"|[0-9][0-9]\/[0-9][0-9]\/[0-9][0-9][0-9][0-9]|"chunked"|"gzip"|"deflate"    return GENERAL_HEADER_VALUE;
"Accept-Charset"|"Refered"|"User-Agent" return REQUEST_HEADER_VALUE;
"Content-Length"|"Expires" return ENTITY_HEADER_VALUE;


[a-zA-Z][a-zA-Z0-9]* return BODY_VALUE;  //TODO να παίρνει και κενά!


[ \t\n]		;
.			printf("unexpected input\n");

%%

int yywrap(void) {
	return 1;
}