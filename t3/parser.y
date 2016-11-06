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
#include "tree.h"
#include "parser.h"
#include "tables.h"

extern int yylineno;
int yylex(void);
void yyerror(char const *s);
int decl_variable (int index, char* type, int scope, int line);
int use_variable  (int index, int scope, int line);
int decl_function (int index, int size_args, int line);
int use_function  (int index, int size_args, int line);

LitTable* LitT;
SymTable* SymT;
FuncTable* FuncT;
AuxTable* AuxT;
Tree *tree;
int scope = 0;
%}

%define api.value.type {Tree*}  /* Type of variable yylval; */

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

program:
  func-decl-list                            { tree = $1; }

func-decl-list:
  func-decl-list func-decl                  { $$ = $1;  add_child($1, $2); }
| func-decl                                 { $$ = new_subtree(FUNC_DECL_LIST_NODE, 1, $1); }

func-decl:
  func-header func-body                     { $$ = new_subtree(FUNC_DECL_NODE, 2, $1, $2);  scope++;}

func-header:
  ret-type ID LPAREN params RPAREN          { $$ = new_subtree(FUNC_HEADER_NODE, 3, $1, $2, $4); int index = get_index($2); int params_size = get_children_size($4); index = decl_function (index, params_size, yylineno); set_type($2, "id"); set_index($2, index);}

func-body:
  LBRACE opt-var-decl opt-stmt-list RBRACE   { $$ = new_subtree(FUNC_BODY_NODE, 2, $2, $3); }

opt-var-decl:
  %empty                                     {  $$ = new_subtree(VAR_LIST_NODE, 0); }
| var-decl-list                              {  $$ = $1; }

opt-stmt-list:
  %empty                                     {  $$ = new_subtree(BLOCK_NODE, 0); }
| stmt-list                                  {  $$ = $1; }

ret-type:
  INT                      { $$ = $1; }
| VOID                     { $$ = $1; }

params:
  VOID                     { $$ = new_subtree(PARAM_LIST_NODE, 0); }
| param-list               { $$ = $1; }

param-list:
  param-list COMMA param   { $$ = $1; add_child($1, $3); }
| param                    { $$ = new_subtree(PARAM_LIST_NODE, 1, $1); }

param:
  INT ID                   { $$ = $2; int index = get_index($2); index = decl_variable (index, "svar", scope, yylineno); set_type($2, "svar"); set_index($2, index);}
| INT ID LBRACK RBRACK     { $$ = $2; int index = get_index($2); index = decl_variable (index, "cvar", scope, yylineno); set_type($2, "cvar"); set_index($2, index);}

var-decl-list:
  var-decl-list var-decl   { $$ = $1; add_child($1, $2); }
| var-decl                 { $$ = new_subtree(VAR_DECL_LIST_NODE, 1, $1); }

var-decl:
  INT ID SEMI                     { $$ = $2;                    int index = get_index($2); index = decl_variable (index, "svar", scope, yylineno); set_type($2, "svar"); set_index($2, index);}
| INT ID LBRACK NUM RBRACK SEMI   { $$ = $2; add_child($2, $4); int index = get_index($2); index = decl_variable (index, "cvar", scope, yylineno); set_type($2, "cvar"); set_index($2, index);}

stmt-list:
  stmt-list stmt       { $$ = $1; add_child($1, $2); }
  | stmt               { $$ = new_subtree(BLOCK_NODE, 1, $1); }

stmt:
  assign-stmt        { $$ = $1; }
| if-stmt            { $$ = $1; }
| while-stmt         { $$ = $1; }
| return-stmt        { $$ = $1; }
| func-call SEMI     { $$ = $1; }

assign-stmt:
  lval ASSIGN arith-expr SEMI { $$ = new_subtree(ASSIGN_NODE, 2, $1, $3); }

lval:
  ID                      { $$ = $1;                    int index = get_index($1); index = use_variable (index, scope, yylineno); set_index($1, index); set_type($1, "svar");}
| ID LBRACK ID RBRACK     { $$ = $1; add_child($1, $3); int index = get_index($1); index = use_variable (index, scope, yylineno); set_index($1, index); set_type($1, "cvar");
                                                            index = get_index($3); index = use_variable (index, scope, yylineno); set_index($3, index); set_type($3, "svar");}
| ID LBRACK NUM RBRACK    { $$ = $1; add_child($1, $3); int index = get_index($1); index = use_variable (index, scope, yylineno); set_index($1, index); set_type($1, "cvar");}

if-stmt:
  IF LPAREN bool-expr RPAREN block               { $$ = new_subtree(IF_NODE, 2, $3, $5);     }
| IF LPAREN bool-expr RPAREN block ELSE block    { $$ = new_subtree(IF_NODE, 3, $3, $5, $7); }

block:
  LBRACE opt-stmt-list RBRACE                    { $$ = $2; }

while-stmt:
  WHILE LPAREN bool-expr RPAREN block            { $$ = new_subtree(WHILE_NODE, 2, $3, $5);  }

return-stmt:
  RETURN SEMI                                    { $$ = new_subtree(RETURN_NODE, 0);         }
| RETURN arith-expr SEMI                         { $$ = new_subtree(RETURN_NODE, 1, $2);     }

func-call:
  output-call     { $$ = $1; }
| write-call      { $$ = $1; }
| user-func-call  { $$ = $1; }

input-call:
  INPUT LPAREN RPAREN                  { $$ = new_subtree(INPUT_NODE, 0); }

output-call:
  OUTPUT LPAREN arith-expr RPAREN      { $$ = new_subtree(OUTPUT_NODE, 1, $3); }

write-call:
  WRITE LPAREN STRING RPAREN           { $$ = new_subtree(WRITE_NODE, 1, $3); }

user-func-call:
  ID LPAREN opt-arg-list RPAREN        { $$ = new_subtree(USER_FUNC_CALL_NODE, 1, $3); int index = get_index($1); index = use_function(index, get_children_size($3), yylineno); set_index($1, index); set_index($$, index); } //TODO TÀ DANDO ERRO

opt-arg-list:
  %empty   { $$ = new_subtree(ARG_LIST_NODE, 0);}
| arg-list { $$ = $1; }

arg-list:
  arg-list COMMA arith-expr { $$ = $1; add_child($1, $3);}
| arith-expr                { $$ = new_subtree(ARG_LIST_NODE, 1, $1);}

bool-expr:
  arith-expr LT arith-expr  { $$ = new_subtree(LT_NODE , 2, $1, $3); }
| arith-expr LE arith-expr  { $$ = new_subtree(LE_NODE , 2, $1, $3); }
| arith-expr GT arith-expr  { $$ = new_subtree(GT_NODE , 2, $1, $3); }
| arith-expr GE arith-expr  { $$ = new_subtree(GE_NODE , 2, $1, $3); }
| arith-expr EQ arith-expr  { $$ = new_subtree(EQ_NODE , 2, $1, $3); }
| arith-expr NEQ arith-expr { $$ = new_subtree(NEQ_NODE, 2, $1, $3); }

arith-expr:
  arith-expr PLUS arith-expr  { $$ = new_subtree(PLUS_NODE , 2, $1, $3); }
| arith-expr MINUS arith-expr { $$ = new_subtree(MINUS_NODE, 2, $1, $3); }
| arith-expr TIMES arith-expr { $$ = new_subtree(TIMES_NODE, 2, $1, $3); }
| arith-expr OVER arith-expr  { $$ = new_subtree(OVER_NODE , 2, $1, $3); }
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

int decl_variable (int index, char* type, int scope, int line){
  char* name = get_id(AuxT, index);
  int i = lookup_var(SymT, name, scope);
  if (i == -1) {
    return add_var(SymT, name, type, scope, line);
  }
  else {
    printf("SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n", line, name, get_var_line(SymT, i));
    exit(-1);
  }
  return -1;
}

int use_variable (int index, int scope, int line) {
  char* name = get_id(AuxT, index);
  int i = lookup_var(SymT, name, scope);
  if(i == -1){
    printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n", line, name);
    exit(-1);
  }
  return i;
}

int decl_function (int index, int args, int line){
  char* name = get_id(AuxT, index);
  int i = lookup_func(FuncT, name, args);
  if (i == -1) {
    return add_func(FuncT, name, args, line);
  }
  else {
    printf("SEMANTIC ERROR (%d): function '%s' already declared at line %d.\n", line, name, get_func_line(FuncT, i));
    exit(-1);
  }
  return -1;
}

int use_function (int index, int args, int line) {
  char* name = get_id(AuxT, index);
  int i = lookup_func(FuncT, name, args);
  if(i == -1){
    printf("SEMANTIC ERROR (%d): function '%s' was not declared.\n", line, name);
    exit(-1);
  }
  int n_args = get_func_args(FuncT, i);
  if(args != n_args) {
    printf("SEMANTIC ERROR (%d): function '%s' was called with %d arguments but declared with %d parameters.\n", line, name, args, n_args);
    exit(-1);
  }
  return i;
}

int main() {

  LitT = create_lit_table();
  SymT = create_sym_table();
  FuncT = create_func_table();
  AuxT = create_aux_table();

  if (yyparse() == 0) {
      printf("PARSE SUCESSFUL!\n\n");
      /*
      print_dot(tree);
      */
      print_lit_table(LitT);
      printf("\n\n");
      print_sym_table(SymT);
      printf("\n\n");
      print_func_table(FuncT);
  }

  free_tree(tree);
  free_lit_table(LitT);
  free_sym_table(SymT);
  free_func_table(FuncT);
  free_aux_table(AuxT);

  return 0;
}
