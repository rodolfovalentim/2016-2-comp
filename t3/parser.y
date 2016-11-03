/* Rodolfo Vieira Valentim */
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
#include <stdlib.h>
#include <stdio.h>
#include "bt.h"
#include "parser.h"
#include "tables.h"

extern int yylineno;
int yylex(void);
void yyerror(char const *s);
void decl_variable (int index, Var_Type type, int line);
void use_variable  (int index, int line);

LitTable* LitT;
SymTable* SymT;
SymTable* SymT_aux;
Var_Type last_type = T_UNDEFINED;
BT *tree;
%}

%define api.value.type {BT*}  /* Type of variable yylval; */

%token ELSE
%token IF
%token INPUT
%token INT
%token OUTPUT
%token RETURN
%token VOID
%token WHILE
%token WRITE
%left PLUS MINUS
%left TIMES OVER
%left LT LE GT GE EQ NEQ
%right ASSIGN
%token SEMI
%token COMMA
%token LPAREN
%token RPAREN
%token LBRACK
%token RBRACK
%token LBRACE
%token RBRACE
%token NUM
%token ID
%token STRING


%%

program: func-decl-list

func-decl-list:
  func-decl-list func-decl
| func-decl

func-decl: func-header func-body

func-header: ret-type ID LPAREN params RPAREN

func-body: LBRACE opt-var-decl opt-stmt-list RBRACE

opt-var-decl:
  %empty
| var-decl-list

opt-stmt-list:
  %empty
| stmt-list

ret-type:
  INT
| VOID

params:
  VOID
| param-list

param-list:
  param-list COMMA param
| param

param:
  INT ID
| INT ID LBRACK RBRACK

var-decl-list:
  var-decl-list var-decl
| var-decl

var-decl:
  INT ID SEMI
| INT ID LBRACK NUM RBRACK SEMI

stmt-list:
stmt-list stmt
| stmt

stmt:
  assign-stmt        { $$ = $1; }           
| if-stmt            { $$ = $1; } 
| while-stmt         { $$ = $1; } 
| return-stmt        { $$ = $1; } 
| func-call SEMI     { $$ = $1; }

assign-stmt: lval ASSIGN arith-expr SEMI

lval:
  ID
| ID LBRACK NUM RBRACK
| ID LBRACK ID RBRACK

if-stmt:
  IF LPAREN bool-expr RPAREN block               { $$ = new_node(IF_NODE, 2, $3, $5);     }
| IF LPAREN bool-expr RPAREN block ELSE block    { $$ = new_node(IF_NODE, 3, $3, $5, $7); }

block:
  LBRACE opt-stmt-list RBRACE

while-stmt:
  WHILE LPAREN bool-expr RPAREN block            { $$ = new_node(WHILE_NODE, 2, $3, $5);  }

return-stmt:
  RETURN SEMI                                    { $$ = new_node(RETURN_NODE, 0);         }
| RETURN arith-expr SEMI                         { $$ = new_node(RETURN_NODE, 1, $2);     }

func-call:
  output-call
| write-call
| user-func-call

input-call: INPUT LPAREN RPAREN                  { $$ = new_node(INPUT_NODE, 0); }

output-call: OUTPUT LPAREN arith-expr RPAREN     { $$ = new_node(OUTPUT_NODE, 0);}

write-call: WRITE LPAREN STRING RPAREN           { $$ = new_node(WRITE_NODE, 0); }

user-func-call: ID LPAREN opt-arg-list RPAREN

opt-arg-list: %empty | arg-list

arg-list:
  arg-list COMMA arith-expr
| arith-expr

bool-expr:
  arith-expr LT arith-expr  { $$ = new_node(LT_NODE , 2, $1, $3); }
| arith-expr LE arith-expr  { $$ = new_node(LE_NODE , 2, $1, $3); }
| arith-expr GT arith-expr  { $$ = new_node(GT_NODE , 2, $1, $3); }
| arith-expr GE arith-expr  { $$ = new_node(GE_NODE , 2, $1, $3); }
| arith-expr EQ arith-expr  { $$ = new_node(EQ_NODE , 2, $1, $3); }
| arith-expr NEQ arith-expr { $$ = new_node(NEQ_NODE, 2, $1, $3); }

arith-expr:
  arith-expr PLUS arith-expr  { $$ = new_node(PLUS_NODE , 2, $1, $3); }
| arith-expr MINUS arith-expr { $$ = new_node(MINUS_NODE, 2, $1, $3); }
| arith-expr TIMES arith-expr { $$ = new_node(TIMES_NODE, 2, $1, $3); }
| arith-expr OVER arith-expr  { $$ = new_node(OVER_NODE , 2, $1, $3); }
| LPAREN arith-expr RPAREN    { $$ = $2; }
| lval                        { $$ = $1; }
| input-call                  { $$ = $1; }
| user-func-call              { $$ = $1; }
| NUM                         { $$ = $1; }

%%
// Error handling.
void yyerror (char const *s) {
  printf("PARSE ERROR (%d): %s\n", yylineno, s);
  exit(1);
}

int main() {
  if(yyparse() == 0) {
    printf("PARSE SUCESSFUL!\n");
        print_tree(tree);
        free_tree(tree);
  }
  return 0;
}
