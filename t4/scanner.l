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
#include "tree.h"
#include "parser.h"
#include "tables.h"

void process_error();
void string_processing(char* string);

extern LitTable* LitT;
extern SymTable* SymT;
extern FuncTable* FuncT;
extern AuxTable* AuxT;

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

"if"			    { yylval = new_node(IF_NODE); return IF;         }
"else"			  { yylval = new_node(ELSE_NODE); return ELSE;     }
"input"			  { yylval = new_node(INPUT_NODE); return INPUT;   }
"int"			    { yylval = new_node(INT_NODE); return INT;       }
"output"		  { yylval = new_node(OUTPUT_NODE); return OUTPUT; }
"return"		  { yylval = new_node(RETURN_NODE); return RETURN; }
"void"			  { yylval = new_node(VOID_NODE); return VOID;     }
"while"			  { yylval = new_node(WHILE_NODE); return WHILE;   }
"write"			  { yylval = new_node(WRITE_NODE); return WRITE;   }
"+"			      { yylval = new_node(PLUS_NODE); return PLUS;     }
"-"			      { yylval = new_node(MINUS_NODE); return MINUS;   }
"*"			      { yylval = new_node(TIMES_NODE); return TIMES;   }
"/"			      { yylval = new_node(OVER_NODE); return OVER;     }
"<"			      { yylval = new_node(LT_NODE); return LT;         }
"<="          { yylval = new_node(LE_NODE); return LE;         }
">"           { yylval = new_node(GT_NODE); return GT;         }
">="          { yylval = new_node(GE_NODE); return GE;         }
"=="          { yylval = new_node(EQ_NODE); return EQ;         }
"!="          { yylval = new_node(NEQ_NODE); return NEQ;       }
"="			      { yylval = new_node(ASSIGN_NODE); return ASSIGN; }
";"			      { yylval = new_node(SEMI_NODE); return SEMI;     }
","			      { yylval = new_node(COMMA_NODE); return COMMA;   }
"("			      { yylval = new_node(LPAREN_NODE); return LPAREN; }
")"			      { yylval = new_node(RPAREN_NODE); return RPAREN; }
"["			      { yylval = new_node(LBRACK_NODE); return LBRACK; }
"]"			      { yylval = new_node(RBRACK_NODE); return RBRACK; }
"}"			      { yylval = new_node(RBRACE_NODE); return RBRACE; }
"{"			      { yylval = new_node(LBRACE_NODE); return LBRACE; }

{number}			{ yylval = new_node(NUM_NODE);    set_type(yylval, "num");    set_index(yylval, atoi(yytext));                                           return NUM;    }
{identifier}  { yylval = new_node(ID_NODE);     set_type(yylval, "id");     int index = add_id(AuxT, yytext);                set_index(yylval, index); return ID;     }
{string}			{ yylval = new_node(STRING_NODE); set_type(yylval, "string"); string_processing(yytext); int index = add_literal(LitT, yytext, "string"); set_index(yylval, index); return STRING; }

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

void string_processing(char* string){
  char tmp[10000];
	int i;
  for(i = 1; string[i+1] != '\0'; i++){
    if((string[i] == 92) && (string[i+1] == 'n')) {
      tmp[i-1] = 10;
      i = i + 1;
    }
    else {
      tmp[i-1] = string[i];
    }
  }
  tmp[i-1] = '\0';
	strcpy(string, tmp);
	/* I have no idea why a have to do this */
	if (string[strlen(string) - 1] == 32 && string[strlen(string) - 2] == 10)
		string[strlen(string) - 1] = 0;
}
