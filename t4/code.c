#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "code.h"
#include "stack.h"
#include "tree.h"
#include "tables.h"

#define MAX_SIZE_TMP 100

extern LitTable* LitT;
extern SymTable* SymT;
extern FuncTable* FuncT;
extern AuxTable* AuxT;

Stack *stack;
int declaration;

int tmp[MAX_SIZE_TMP];
int tmp_size = 1;

int new_tmp(int inc) {
        int old_value = tmp_size;
        tmp_size+=inc;
        return old_value;
}

void rec_emit_ast(Tree *ast);

void emit_func_decl_list(Tree *ast){
        int index = lookup_func(FuncT, "main", 0);
        Tree* addr_main = get_func_address(FuncT, index);
        rec_emit_ast(addr_main);
}

void emit_func_decl(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
}

void emit_func_header(Tree *ast){
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_emit_ast(get_child(ast, i));
        }
}

void emit_int(Tree *ast){

}

void emit_void(Tree *ast){

}

void emit_id(Tree *ast){
        char* type = get_type(ast);
        if(!strcmp(type, "svar")) {
                if(declaration) {
                        set_var_memoffset(SymT, get_index(ast), new_tmp(1));
                }
                else {
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, tmp[offset]);
                }
        }
        else if(!strcmp(type, "cvar")) {
                rec_emit_ast(get_child(ast, 0));
                int size = pop(stack);
                if(declaration) {
                        set_var_memoffset(SymT, get_index(ast), new_tmp(size));
                }
                else {
                        rec_emit_ast(get_child(ast, 0));
                        int index = pop(stack);
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, tmp[index + offset]);
                }
        }
}

void emit_num(Tree *ast){
        push(stack, get_index(ast));
}

void emit_param_list(Tree *ast) {
        declaration = 1;
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_emit_ast(get_child(ast, i));
                int offset = get_var_memoffset(SymT, get_index(get_child(ast, i)));
                tmp[offset] = pop(stack);
        }
        declaration = 0;
}

void emit_func_body(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
}

void emit_var_decl_list(Tree *ast){
        declaration = 1;
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_emit_ast(get_child(ast, i));
        }
        declaration = 0;
}

void emit_block(Tree *ast){
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_emit_ast(get_child(ast, i));
        }
}

void emit_input(Tree *ast){
        int i;
        scanf("%d", &i);
        push(stack, i);
}

void emit_output(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        int i = pop(stack);
        printf("%d\n", i);
}

void emit_user_func_call(Tree *ast){
        int index = get_index(ast);
        Tree* addr_func = get_func_address(FuncT, index);
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(addr_func);
}

void emit_arg_list(Tree *ast){
        for(int i = get_children_size(ast); i > 0; i--) {
                rec_emit_ast(get_child(ast, i-1));
        }
}

void emit_write(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        int i = pop(stack);
        printf("%s", get_literal(LitT, i)); // TODO parser da string
}

void emit_string(Tree *ast){
        push(stack, get_index(ast));
}

void emit_assign(Tree *ast){
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        Tree* var = get_child(ast, 0);
        char* type = get_type(var);
        int offset = get_var_memoffset(SymT, get_index(var));
        if(!strcmp(type, "cvar")) {
                rec_emit_ast(get_child(var, 0));
                offset += pop(stack);
        }
        tmp[offset] = s;
}

void emit_plus(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int r = pop(stack);
        int s = pop(stack);
        push(stack, r+s);
}

void emit_minus(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r-s);
}

void emit_times(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int r = pop(stack);
        int s = pop(stack);
        push(stack, r*s);
}


void emit_over(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r/s);
}

void emit_lt(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r<s);
}

void emit_le(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r<=s);
}

void emit_gt(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r>s);
}

void emit_ge(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r>=s);
}

void emit_eq(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r==s);
}

void emit_neq(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        rec_emit_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r!=s);
}

void emit_if(Tree* ast){
        rec_emit_ast(get_child(ast, 0));
        if(pop(stack)) {
                rec_emit_ast(get_child(ast, 1));
        }
        else {
                rec_emit_ast(get_child(ast, 2));
        }
}

void emit_while(Tree* ast){
        rec_emit_ast(get_child(ast, 0));
        while(pop(stack)) {
                rec_emit_ast(get_child(ast, 1));
                rec_emit_ast(get_child(ast, 0));
        }
}

void emit_return(Tree *ast){
        rec_emit_ast(get_child(ast, 0));
        int value = pop(stack);
        push(stack, value);
}

void rec_emit_ast(Tree *ast) {
        char string[10];
        node2str(ast, string);
        fprintf(stderr, "Kind: %s\n", string);
        switch(get_kind(ast)) {
        case IF_NODE: emit_if(ast); break;
        case INPUT_NODE: emit_input(ast); break;
        case INT_NODE: emit_int(ast); break;
        case OUTPUT_NODE: emit_output(ast); break;
        case RETURN_NODE: emit_return(ast); break;
        case VOID_NODE: emit_void(ast); break;
        case WHILE_NODE: emit_while(ast); break;
        case WRITE_NODE: emit_write(ast); break;
        case PLUS_NODE: emit_plus(ast); break;
        case MINUS_NODE: emit_minus(ast); break;
        case TIMES_NODE: emit_times(ast); break;
        case OVER_NODE: emit_over(ast); break;
        case LT_NODE: emit_lt(ast); break;
        case LE_NODE: emit_le(ast); break;
        case GT_NODE: emit_gt(ast); break;
        case GE_NODE: emit_ge(ast); break;
        case EQ_NODE: emit_eq(ast); break;
        case NEQ_NODE: emit_neq(ast); break;
        case ASSIGN_NODE: emit_assign(ast); break;
        case NUM_NODE: emit_num(ast); break;
        case ID_NODE: emit_id(ast); break;
        case STRING_NODE: emit_string(ast); break;
        case FUNC_DECL_LIST_NODE: emit_func_decl_list(ast); break;
        case FUNC_DECL_NODE: emit_func_decl(ast); break;
        case FUNC_HEADER_NODE: emit_func_header(ast); break;
        case FUNC_BODY_NODE: emit_func_body(ast); break;
        case PARAM_LIST_NODE: emit_param_list(ast); break;
        case ARG_LIST_NODE: emit_arg_list(ast); break;
        case BLOCK_NODE: emit_block(ast); break;
        case VAR_DECL_LIST_NODE: emit_var_decl_list(ast); break;
        case USER_FUNC_CALL_NODE: emit_user_func_call(ast); break;
        default:
                node2str(ast, string);
                fprintf(stderr, "Invalid kind: %s\n", string);
                exit(1);
        }
}

void emit_code(Tree *ast) {
        stack = init_stack();
        rec_emit_ast(ast);
}
