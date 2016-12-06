#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "code.h"
#include "stack.h"
#include "tree.h"
#include "tables.h"

#define MAX_SIZE_TMP 99999

extern LitTable* LitT;
extern SymTable* SymT;
extern FuncTable* FuncT;
extern AuxTable* AuxT;

Stack *stack;
/* Variaveis globais (not gambiarra) */
int param_list;
int arg_list;
int var_decl;
int left;
Tree* curnt_func_params;
Tree* curnt_param;

int tmp[MAX_SIZE_TMP];
int tmp_size = 0;

int new_tmp(int inc) {
        int old_value = tmp_size;
        tmp_size += inc;
        return old_value;
}

void rec_run_ast(Tree *ast);

void run_func_decl_list(Tree *ast){
        int index = lookup_func(FuncT, "main", 0);
        Tree* addr_main = get_func_address(FuncT, index);
        rec_run_ast(addr_main);
}

void run_func_decl(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
}

void run_func_header(Tree *ast){
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_run_ast(get_child(ast, i));
        }
}

void run_int(Tree *ast){

}

void run_void(Tree *ast){

}

void run_id(Tree *ast) {
        char* type = get_type(ast);
        if(var_decl) {
                if(!strcmp(type, "svar")) {
                        set_var_memoffset(SymT, get_index(ast), new_tmp(1));
                }
                else if(!strcmp(type, "cvar")) {
                        rec_run_ast(get_child(ast, 0));
                        int size = pop(stack);
                        set_var_memoffset(SymT, get_index(ast), new_tmp(size));
                }
        }
        else if(arg_list) {
                /* Workaround to know the parameter type */
                type = get_type(curnt_param);
                if(!strcmp(type, "svar")) {
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, tmp[offset]);
                }
                else if(!strcmp(type, "cvar")) {
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        if(get_children_size(ast) == 0) {
                                push(stack, offset);
                        }
                        else {
                                rec_run_ast(get_child(ast, 0));
                                int size = pop(stack);
                                push(stack, tmp[offset + size]);
                        }
                }
        }
        else if(param_list) {
                if(!strcmp(type, "svar")) {
                        int offset = new_tmp(1);
                        set_var_memoffset(SymT, get_index(ast), offset);
                        tmp[offset] = pop(stack);

                }
                else if(!strcmp(type, "cvar")) {
                        set_var_memoffset(SymT, get_index(ast), pop(stack));
                }
        }
        else if(left) {
                if(!strcmp(type, "svar")) {
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, offset);
                }
                else if(!strcmp(type, "cvar")) {
                        left = 0;
                        rec_run_ast(get_child(ast, 0));
                        left = 1;
                        int index = pop(stack);
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, offset + index);
                }
        }
        else {
                if(!strcmp(type, "svar")) {
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, tmp[offset]);
                }
                else if(!strcmp(type, "cvar")) {
                        rec_run_ast(get_child(ast, 0));
                        int index = pop(stack);
                        int offset = get_var_memoffset(SymT, get_index(ast));
                        push(stack, tmp[index + offset]);
                }
        }
}

void run_param_list(Tree *ast) {
        param_list = 1;
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_run_ast(get_child(ast, i));
        }
        param_list = 0;
}

void run_num(Tree *ast){
        push(stack, get_index(ast));
}

void run_func_body(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
}

void run_var_decl_list(Tree *ast){
        var_decl = 1;
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_run_ast(get_child(ast, i));
        }
        var_decl = 0;
}

void run_block(Tree *ast){
        for(int i = 0; i < get_children_size(ast); i++) {
                rec_run_ast(get_child(ast, i));
        }
}

void run_input(Tree *ast){
        int i;
        scanf("%d", &i);
        push(stack, i);
}

void run_output(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        int i = pop(stack);
        printf("%d", i);
}

void run_user_func_call(Tree *ast){
        int index = get_index(ast);
        Tree* addr_func = get_func_address(FuncT, index);
        curnt_func_params = get_child(get_child(addr_func, 0), 2); //Very ugly
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(addr_func);
}

void run_arg_list(Tree *ast){
        arg_list = 1;
        for(int i = get_children_size(ast); i > 0; i--) {
                curnt_param = get_child(curnt_func_params, i-1);
                rec_run_ast(get_child(ast, i-1));
        }
        arg_list = 0;
}

void run_write(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        int i = pop(stack);
        printf("%s", get_literal(LitT, i)); // TODO parser da string
}

void run_string(Tree *ast){
        push(stack, get_index(ast));
}

void run_assign(Tree *ast){
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        left = 1;
        rec_run_ast(get_child(ast, 0));
        left = 0;
        int offset = pop(stack);
        tmp[offset] = s;
}

void run_plus(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int r = pop(stack);
        int s = pop(stack);
        push(stack, r+s);
}

void run_minus(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r-s);
}

void run_times(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int r = pop(stack);
        int s = pop(stack);
        push(stack, r*s);
}


void run_over(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r/s);
}

void run_lt(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r<s);
}

void run_le(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r<=s);
}

void run_gt(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r>s);
}

void run_ge(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r>=s);
}

void run_eq(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r==s);
}

void run_neq(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        rec_run_ast(get_child(ast, 1));
        int s = pop(stack);
        int r = pop(stack);
        push(stack, r!=s);
}

void run_if(Tree* ast){
        rec_run_ast(get_child(ast, 0));
        if(pop(stack)) {
                rec_run_ast(get_child(ast, 1));
        }
        else if (get_child(ast, 2) != NULL) {
                rec_run_ast(get_child(ast, 2));
        }
}

void run_while(Tree* ast){
        rec_run_ast(get_child(ast, 0));
        while(pop(stack)) {
                rec_run_ast(get_child(ast, 1));
                rec_run_ast(get_child(ast, 0));
        }
}

void run_return(Tree *ast){
        rec_run_ast(get_child(ast, 0));
        int value = pop(stack);
        push(stack, value);
}

void rec_run_ast(Tree *ast) {
        char string[10];
        switch(get_kind(ast)) {
        case IF_NODE: run_if(ast); break;
        case INPUT_NODE: run_input(ast); break;
        case INT_NODE: run_int(ast); break;
        case OUTPUT_NODE: run_output(ast); break;
        case RETURN_NODE: run_return(ast); break;
        case VOID_NODE: run_void(ast); break;
        case WHILE_NODE: run_while(ast); break;
        case WRITE_NODE: run_write(ast); break;
        case PLUS_NODE: run_plus(ast); break;
        case MINUS_NODE: run_minus(ast); break;
        case TIMES_NODE: run_times(ast); break;
        case OVER_NODE: run_over(ast); break;
        case LT_NODE: run_lt(ast); break;
        case LE_NODE: run_le(ast); break;
        case GT_NODE: run_gt(ast); break;
        case GE_NODE: run_ge(ast); break;
        case EQ_NODE: run_eq(ast); break;
        case NEQ_NODE: run_neq(ast); break;
        case ASSIGN_NODE: run_assign(ast); break;
        case NUM_NODE: run_num(ast); break;
        case ID_NODE: run_id(ast); break;
        case STRING_NODE: run_string(ast); break;
        case FUNC_DECL_LIST_NODE: run_func_decl_list(ast); break;
        case FUNC_DECL_NODE: run_func_decl(ast); break;
        case FUNC_HEADER_NODE: run_func_header(ast); break;
        case FUNC_BODY_NODE: run_func_body(ast); break;
        case PARAM_LIST_NODE: run_param_list(ast); break;
        case ARG_LIST_NODE: run_arg_list(ast); break;
        case BLOCK_NODE: run_block(ast); break;
        case VAR_DECL_LIST_NODE: run_var_decl_list(ast); break;
        case USER_FUNC_CALL_NODE: run_user_func_call(ast); break;
        default:
                node2str(ast, string);
                fprintf(stderr, "Invalid kind: %s\n", string);
                exit(1);
        }
}

void print_tmp(){
        int i;
        for(i = 0; i < tmp_size; i++) {
                fprintf(stderr, "%d:%d ", i, tmp[i]);
        }
        fprintf(stderr, "\n");
}

void run_code(Tree *ast) {
        stack = init_stack();
        rec_run_ast(ast);
        //print_tmp();
}
