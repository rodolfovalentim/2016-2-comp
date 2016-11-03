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
#include "bt.h"
#include "parser.h"
#include "tables.h"


void process_error();

extern LitTable* LitT;
extern SymTable* SymT;
extern SymTable* SymT_aux;
extern Var_Type last_type;

//TODO se as variaveis/funcoes utilizadas foram declaradas;
//TODO se há variaveis/funcoes redeclaradas; e
//TODO se o numero de argumentos (aridade) de uma chamada de funcao esta correto.
//NOTE SEMANTIC ERROR (XX): variable ’VV’ was not declared.
//NOTE SEMANTIC ERROR (XX): function ’FF’ was not declared.
//NOTE SEMANTIC ERROR (XX): variable ’VV’ already declared at line YY.
//NOTE SEMANTIC ERROR (XX): function ’FF’ already declared at line YY.
//NOTE SEMANTIC ERROR (XX): function ’FF’ was called with AA arguments but declared with PP parameters.
%}


number		  [0-9]+
identifier	[a-zA-Z][a-zA-Z0-9]*
string		  \"(.*)\"
inline		  "//"(.*)

/* Multi-line comments are handled in the rules section. */
/* First we need to define a start condition.            */

%x comment
comments   "/*"

%%

"if"			    { yylval = new_leaf(IF_NODE, yytext); return IF;         }
"else"			  { yylval = new_leaf(ELSE_NODE, yytext); return ELSE;     }
"input"			  { yylval = new_leaf(INPUT_NODE, yytext); return INPUT;   }
"int"			    { yylval = new_leaf(INT_NODE, yytext); return INT;       }
"output"		  { yylval = new_leaf(OUTPUT_NODE, yytext); return OUTPUT; }
"return"		  { yylval = new_leaf(RETURN_NODE, yytext); return RETURN; }
"void"			  { yylval = new_leaf(VOID_NODE, yytext); return VOID;     }
"while"			  { yylval = new_leaf(WHILE_NODE, yytext); return WHILE;   }
"write"			  { yylval = new_leaf(WRITE_NODE, yytext); return WRITE;   }
"+"			      { yylval = new_leaf(PLUS_NODE, yytext); return PLUS;     }
"-"			      { yylval = new_leaf(MINUS_NODE, yytext); return MINUS;   }
"*"			      { yylval = new_leaf(TIMES_NODE, yytext); return TIMES;   }
"/"			      { yylval = new_leaf(OVER_NODE, yytext); return OVER;     }
"<"			      { yylval = new_leaf(LT_NODE, yytext); return LT;         }
"<="          { yylval = new_leaf(LE_NODE, yytext); return LE;         }
">"           { yylval = new_leaf(GT_NODE, yytext); return GT;         }
">="          { yylval = new_leaf(GE_NODE, yytext); return GE;         }
"=="          { yylval = new_leaf(EQ_NODE, yytext); return EQ;         }
"!="          { yylval = new_leaf(NEQ_NODE, yytext); return NEQ;       }
"="			      { yylval = new_leaf(ASSIGN_NODE, yytext); return ASSIGN; }
";"			      { yylval = new_leaf(SEMI_NODE, yytext); return SEMI;     }
","			      { yylval = new_leaf(COMMA_NODE, yytext); return COMMA;   }
"("			      { yylval = new_leaf(LPAREN_NODE, yytext); return LPAREN; }
")"			      { yylval = new_leaf(RPAREN_NODE, yytext); return RPAREN; }
"["			      { yylval = new_leaf(LBRACK_NODE, yytext); return LBRACK; }
"]"			      { yylval = new_leaf(RBRACK_NODE, yytext); return RBRACK; }
"}"			      { yylval = new_leaf(RBRACE_NODE, yytext); return RBRACE; }
"{"			      { yylval = new_leaf(LBRACE_NODE, yytext); return LBRACE; }

{number}			{ yylval = new_leaf(NUM_NODE, yytext); return NUM;      }
{identifier}  { yylval = new_leaf(ID_NODE, yytext); return ID;        }
{string}			{ yylval = new_leaf(STRING_NODE, yytext); return STRING;}


[ \t]+			                     /* eat up whitespace */
"\n"			           yylineno++; /* eat up a single line */

{inline}			                   /* ignore inline comments */

{comments}			  { BEGIN(comment); }
<comment>"*/"		    BEGIN(INITIAL);
<comment>[^*\n]+
<comment>"*"
<comment>\n			    yylineno++;

.			            { process_error(); } /* Be sure to keep this as the last rule */

%%

void process_error() {
	printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext);
    exit(1);
}
