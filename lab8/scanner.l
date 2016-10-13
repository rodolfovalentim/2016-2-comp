/* My implementation of Lab07 */

%option outfile="scanner.c"
%option noyywrap
%option nounput
%option noinput

%{
#include "bt.h"
#include "parser.h"

#define process_token(type) return type
%}

number      [0-9]+
identifier  [a-zA-Z]+

/* Multi-line comments are handled in the rules section.
   First we need to define a start condition. */
%x          ML_COMMENT

%%

"if"            { yylval = new_leaf(IF_NODE, yytext); return IF;}
"then"          { yylval = new_leaf(THEN_NODE, yytext); return THEN;}
"else"          { yylval = new_leaf(ELSE_NODE, yytext); return ELSE;}
"end"           { yylval = new_leaf(END_NODE, yytext); return END;}
"repeat"        { yylval = new_leaf(REPEAT_NODE, yytext); return REPEAT;}
"until"         { yylval = new_leaf(UNTIL_NODE, yytext); return UNTIL;}
"read"          { yylval = new_leaf(READ_NODE, yytext); return READ;}
"write"         { yylval = new_leaf(WRITE_NODE, yytext); return WRITE;}

"+"             { yylval = new_leaf(PLUS_NODE, yytext); return PLUS;}
"-"             { yylval = new_leaf(MINUS_NODE, yytext); return MINUS;}
"*"             { yylval = new_leaf(TIMES_NODE, yytext); return TIMES;}
"/"             { yylval = new_leaf(OVER_NODE, yytext); return OVER;}

"="             { yylval = new_leaf(EQ_NODE, yytext); return EQ;}
"<"             { yylval = new_leaf(LT_NODE, yytext); return LT;}

"("             { yylval = new_leaf(LPAREN_NODE, yytext); return LPAREN;}
")"             { yylval = new_leaf(RPAREN_NODE, yytext); return RPAREN;}
";"             { yylval = new_leaf(SEMI_NODE, yytext); return SEMI;}

":="            { yylval = new_leaf(ASSIGN_NODE, yytext); return ASSIGN;}

{number}        { yylval = new_leaf(NUM_NODE, yytext); return NUM;}
{identifier}    { yylval = new_leaf(ID_NODE, yytext); return ID;}

[ \t]+          /* eat up whitespace */
"\n"            yylineno++; /* eat up a single line */

"{"                 BEGIN(ML_COMMENT); /* multi-line comments */
<ML_COMMENT>[^}\n]* /* eat anything that’s not a ’}’ */
<ML_COMMENT>"\n"    yylineno++;
<ML_COMMENT>"}"     BEGIN(INITIAL);

                /* Be sure to keep this as the last rule */
.               { printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext);
                  exit(1); }

%%
