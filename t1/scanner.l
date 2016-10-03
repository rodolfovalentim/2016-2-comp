/* Rodolfo Vieira Valentim */

/* Options to flex */
%option outfile="scanner.c"

/* Tell flex to don't declare this functions to avoid compilation warnings.
 * They are not necessary for the kind of scanner we need.
**/

%option noyywrap
%option nounput
%option noinput

%{
#include "token.h"

void process_token(TokenType);
void process_error();
%}


number		[0-9]+
identifier	[a-zA-Z][a-zA-Z0-9]*
string		\"(.*)\"
inline		"//"(.*)	

/* Multi-line comments are handled in the rules section.
   First we need to define a start condition. */

%x comment
comments "/*"

%%

"else"			{ process_token(ELSE); }
"if"			{ process_token(IF); }	
"input"			{ process_token(INPUT); }
"int"			{ process_token(INT); }
"output"			{ process_token(OUTPUT); }
"return"			{ process_token(RETURN); }
"void"			{ process_token(VOID); }
"while"			{ process_token(WHILE); }
"write"			{ process_token(WRITE); }
"+"			{ process_token(PLUS); }
"-"			{ process_token(MINUS); }
"*"			{ process_token(TIMES); }
"/"			{ process_token(OVER); }
"<"			{ process_token(LT); }
"<="			{ process_token(LE); }
">" 			{ process_token(GT); }
">="			{ process_token(GE); }
"=="			{ process_token(EQ); }
"!="			{ process_token(NEQ); }
"="			{ process_token(ASSIGN); }
";"			{ process_token(SEMI); }
","			{ process_token(COMMA); }
"("			{ process_token(LPAREN); }
")"			{ process_token(RPAREN); }
"["			{ process_token(LBRACK); }
"]"			{ process_token(RBRACK); }
"{"			{ process_token(LBRACE); }
"}"			{ process_token(RBRACE); }

{number}			{ process_token(NUM); }
{identifier}			{ process_token(ID); }
{string}			{ process_token(STRING); }


[ \t]+			/* eat up whitespace */
"\n"			yylineno++; /* eat up a single line */

{inline}			/* ignore inline comments */

{comments}			{ BEGIN(comment); }
<comment>"*/"			BEGIN(INITIAL); 
<comment>[^*\n]+
<comment>"*"
<comment>\n			yylineno++;

.			{ process_error(); } /* Be sure to keep this as the last rule */

%%

void process_token(TokenType tk) {
	printf("%d: %s -> %s\n", yylineno, yytext, TOKEN_TYPE_STRING[tk]);
}

void process_error() {
	printf("%d: %s -> %s\n", yylineno, yytext, "UNKNOWN");
}

int main(void) {
	yylex();
	return 0;
}
