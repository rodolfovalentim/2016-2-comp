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
#include "parser.h"
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

"else"			{ return ELSE;  }
"if"			{ return IF;  }
"input"			{ return INPUT;  }
"int"			{ return INT;  }
"output"		{ return OUTPUT;  }
"return"		{ return RETURN;  }
"void"			{ return VOID;  }
"while"			{ return WHILE;  }
"write"			{ return WRITE;  }
"+"			{ return PLUS;  }
"-"			{ return MINUS;  }
"*"			{ return TIMES;  }
"/"			{ return OVER;  }
"<"			{ return LT;  }
"<="			{ return LE;  }
">" 			{ return GT;  }
">="			{ return GE;  }
"=="			{ return EQ;  }
"!="			{ return NEQ;  }
"="			{ return ASSIGN;  }
";"			{ return SEMI;  }
","			{ return COMMA;  }
"("			{ return LPAREN;  }
")"			{ return RPAREN;  }
"["			{ return LBRACK;  }
"]"			{ return RBRACK;  }
"{"			{ return LBRACE;  }
"}"			{ return RBRACE;  }

{number}			{ return NUM;  }
{identifier}		{ return ID;  }
{string}			{ return STRING;  }


[ \t]+			/* eat up whitespace */
"\n"			yylineno++; /* eat up a single line */

{inline}			/* ignore inline comments */

{comments}			{ BEGIN(comment); }
<comment>"*/"		BEGIN(INITIAL);
<comment>[^*\n]+
<comment>"*"
<comment>\n			yylineno++;

.			{ process_error(); } /* Be sure to keep this as the last rule */

%%

void process_error() {
	printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext);
    exit(1);
}
