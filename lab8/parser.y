// My implementation of Lab07.

/* Options to bison */
// File name of generated parser.
%output "parser.c"
// Produces a 'parser.h'
%defines "parser.h"
// Give proper error messages when a syntax error is found.
%define parse.error verbose
// Enable LAC (lookahead correction) to improve syntax error handling.
%define parse.lac full

%{
#include <stdio.h>
#include "bt.h"
#include "parser.h"

int yylex();
void yyerror(const char *s);

BT *tree;
extern int yylineno;
%}
%define api.value.type {BT*} // Type of variable yylval;

%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%token SEMI LPAREN RPAREN
%token ASSIGN

%token NUM
%token ID

%left LT EQ
// Precedence of operators (* and / have higher precedence).
// All four operators are left associative.
%left PLUS MINUS
%left TIMES OVER

// Start symbol for the grammar.
%start program

%%

program:
  stmt_sequence { tree = new_node(PROGRAM, 1, $1); }
;

stmt_sequence:
  stmt_sequence stmt { $$ = new_node(STMT_SEQUENCE, 2, $1, $2); }
| stmt { $$ = new_node(STMT_SEQUENCE, 1, $1); }
;

stmt:
  if_stmt      { $$ = new_node(STMT, 1, $1);  }
| repeat_stmt  { $$ = new_node(STMT, 1, $1);  }
| assign_stmt  { $$ = new_node(STMT, 1, $1);  }
| read_stmt    { $$ = new_node(STMT, 1, $1);  }
| write_stmt   { $$ = new_node(STMT, 1, $1);  }
;

if_stmt:
  IF expr THEN stmt_sequence END { $$ = new_node(IF_STMT, 5, $1, $2, $3, $4, $5); }
| IF expr THEN stmt_sequence ELSE stmt_sequence END { $$ = new_node(IF_STMT, 7, $1, $2, $3, $4, $5, $6, $7); }
;

repeat_stmt:
  REPEAT stmt_sequence UNTIL expr  { $$ = new_node(REPEAT_STMT, 4, $1, $2, $3, $4); }
;

assign_stmt:
  ID ASSIGN expr SEMI  { $$ = new_node(ASSIGN_STMT, 4, $1, $2, $3, $4); }
;

read_stmt:
  READ ID SEMI    { $$ = new_node(READ_STMT, 3, $1, $2, $3); }
;

write_stmt:
  WRITE expr SEMI { $$ = new_node(WRITE_STMT, 3, $1, $2, $3); }
;

expr:
  expr LT expr    { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| expr EQ expr    { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| expr PLUS expr  { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| expr MINUS expr { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| expr TIMES expr { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| expr OVER expr  { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| LPAREN expr RPAREN { $$ = new_node(EXPR_NODE, 3, $1, $2, $3); }
| NUM { $$ = new_node(EXPR_NODE, 1, $1); }
| ID  { $$ = new_node(EXPR_NODE, 1, $1); }
;

%%

// Error handling.
void yyerror (char const *s) {
    printf("PARSE ERROR (%d): %s\n", yylineno, s);
}

// Main.
int main() {
  if (yyparse() == 0) {
      //printf("AST of given expression:\n");
      //print_tree(tree);
      print_dot(tree);
      free_tree(tree);
  }
  return 0;
}
