#ifndef CODE_H
#define CODE_H

#include "tree.h"

// typedef enum {
//     HALT,
//     IN,
//     OUT,
//     ADD,
//     SUB,
//     MUL,
//     DIV,
//     LD,     // Load variable
//     LDA,    // Load address
//     LDC,    // Load constant
//     ST,     // Store
//     JON,    // Jump On Negative
//     JNZ,    // Jump on Not Zero
//     JMP     // Unconditional jump
// } OpCode;
//
// static char* OpStr[] =
// { "HALT", "IN", "OUT", "ADD", "SUB", "MUL", "DIV",
//   "LD", "LDA", "LDC", "ST", "JON", "JNZ", "JMP" };
//
// static int OpCount[] =
// { 0, 1, 1, 3, 3, 3, 3,
//   3, 3, 2, 3, 2, 2, 1 };

// Traverses the given Tree and runs the corresponding TM ASM code to STDOUT.
void run_code(Tree *ast);
void rec_run_code(Tree *ast);

void run_if(Tree* ast);
void run_else(Tree* ast);
void run_input(Tree* ast);
void run_int(Tree* ast);
void run_output(Tree* ast);
void run_return(Tree* ast);
void run_void(Tree* ast);
void run_while(Tree* ast);
void run_write(Tree* ast);
void run_plus(Tree* ast);
void run_minus(Tree* ast);
void run_times(Tree* ast);
void run_over(Tree* ast);
void run_lt(Tree* ast);
void run_le(Tree* ast);
void run_gt(Tree* ast);
void run_ge(Tree* ast);
void run_eq(Tree* ast);
void run_neq(Tree* ast);
void run_assign(Tree* ast);
void run_semi(Tree* ast);
void run_comma(Tree* ast);
void run_lparen(Tree* ast);
void run_rparen(Tree* ast);
void run_lbrack(Tree* ast);
void run_rbrack(Tree* ast);
void run_lbrace(Tree* ast);
void run_rbrace(Tree* ast);
void run_num(Tree* ast);
void run_id(Tree* ast);
void run_string(Tree* ast);
void run_func_decl_list(Tree* ast);
void run_func_decl(Tree* ast);
void run_func_header(Tree* ast);
void run_func_body(Tree* ast);
void run_param_list(Tree* ast);
void run_param(Tree* ast);
void run_var_decl_list(Tree* ast);
void run_stmt_list(Tree* ast);
void run_arg_list(Tree* ast);
void run_block(Tree* ast);
void run_var_list(Tree* ast);
void run_user_func_call(Tree* ast);

#endif
