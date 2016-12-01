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

// Traverses the given Tree and emits the corresponding TM ASM code to STDOUT.
void emit_code(Tree *ast);
void rec_emit_code(Tree *ast);

void emit_if(Tree* ast);
void emit_else(Tree* ast);
void emit_input(Tree* ast);
void emit_int(Tree* ast);
void emit_output(Tree* ast);
void emit_return(Tree* ast);
void emit_void(Tree* ast);
void emit_while(Tree* ast);
void emit_write(Tree* ast);
void emit_plus(Tree* ast);
void emit_minus(Tree* ast);
void emit_times(Tree* ast);
void emit_over(Tree* ast);
void emit_lt(Tree* ast);
void emit_le(Tree* ast);
void emit_gt(Tree* ast);
void emit_ge(Tree* ast);
void emit_eq(Tree* ast);
void emit_neq(Tree* ast);
void emit_assign(Tree* ast);
void emit_semi(Tree* ast);
void emit_comma(Tree* ast);
void emit_lparen(Tree* ast);
void emit_rparen(Tree* ast);
void emit_lbrack(Tree* ast);
void emit_rbrack(Tree* ast);
void emit_lbrace(Tree* ast);
void emit_rbrace(Tree* ast);
void emit_num(Tree* ast);
void emit_id(Tree* ast);
void emit_string(Tree* ast);
void emit_func_decl_list(Tree* ast);
void emit_func_decl(Tree* ast);
void emit_func_header(Tree* ast);
void emit_func_body(Tree* ast);
void emit_param_list(Tree* ast);
void emit_param(Tree* ast);
void emit_var_decl_list(Tree* ast);
void emit_stmt_list(Tree* ast);
void emit_arg_list(Tree* ast);
void emit_block(Tree* ast);
void emit_var_list(Tree* ast);
void emit_user_func_call(Tree* ast);

#endif
