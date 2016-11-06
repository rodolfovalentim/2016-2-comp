
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tables.h"

// Literals Table
// ----------------------------------------------------------------------------

#define TYPE_MAX_SIZE 10
#define LITERAL_MAX_SIZE 128
#define LITERALS_TABLE_MAX_SIZE 100

typedef struct {
        char name[LITERAL_MAX_SIZE];
        char type[TYPE_MAX_SIZE];
        int line;
} L_Entry;

struct lit_table {
        L_Entry t[LITERALS_TABLE_MAX_SIZE];
        int size;
};

LitTable* create_lit_table() {
        LitTable *lt = malloc(sizeof *lt);
        lt->size = 0;
        return lt;
}

int add_literal(LitTable* lt, char* s, char* t) {
        for (int i = 0; i < lt->size; i++) {
                if (strcmp(lt->t[i].name, s) == 0 && strcmp(lt->t[i].type, t) == 0) {
                        return i;
                }
        }
        strcpy(lt->t[lt->size].name, s);
        strcpy(lt->t[lt->size].type, t);
        int old_side = lt->size;
        lt->size++;
        return old_side;
}

char* get_literal(LitTable* lt, int i) {
        return lt->t[i].name;
}

void print_lit_table(LitTable* lt) {
        printf("Literals table:\n");
        for (int i = 0; i < lt->size; i++) {
                printf("Entry %d -- %s\n", i, get_literal(lt, i));
        }
}

void free_lit_table(LitTable* lt) {
        free(lt);
}

// Symbols Table
// ----------------------------------------------------------------------------

#define SYMBOL_MAX_SIZE 128
#define SYMBOL_TABLE_MAX_SIZE 100

typedef struct {
        char name[SYMBOL_MAX_SIZE];
        char type[TYPE_MAX_SIZE];
        int scope;
        int line;
} Entry;

struct sym_table {
        Entry t[SYMBOL_TABLE_MAX_SIZE];
        int size;
};

SymTable* create_sym_table() {
        SymTable *st = malloc(sizeof *st);
        st->size = 0;
        return st;
}

int lookup_var(SymTable* st, char* s, int scope) {
        for (int i = 0; i < st->size; i++) {
                if (strcmp(st->t[i].name, s) == 0 && st->t[i].scope == scope)  {
                        return i;
                }
        }
        return -1;
}

int add_var(SymTable* st, char* s, char* t, int scope, int line) {
        strcpy(st->t[st->size].name, s);
        strcpy(st->t[st->size].type, t);
        st->t[st->size].scope = scope;
        st->t[st->size].line = line;
        int old_side = st->size;
        st->size++;
        return old_side;
}

char* get_var_name(SymTable* st, int i) {
        return st->t[i].name;
}

int get_var_line(SymTable* st, int i) {
        return st->t[i].line;
}

char* get_var_type(SymTable* st, int i) {
        return st->t[i].type;
}

int get_var_scope(SymTable* st, int i) {
        return st->t[i].scope;
}

void print_sym_table(SymTable* st) {
        printf("Symbols table:\n");
        for (int i = 0; i < st->size; i++) {
                printf("Entry %d -- name: %s, line: %d, scope: %d\n", i, get_var_name(st, i), get_var_line(st, i), get_var_scope(st, i));
        }
}

void free_sym_table(SymTable* st) {
        free(st);
}

// Functions Table
// ----------------------------------------------------------------------------
#define SYMBOL_MAX_SIZE 128
#define SYMBOL_TABLE_MAX_SIZE 100

typedef struct {
        char name[SYMBOL_MAX_SIZE];
        int size_args;
        int line;
} F_Entry;


struct func_table {
        F_Entry t[SYMBOL_TABLE_MAX_SIZE];
        int size;
};

FuncTable* create_func_table() {
        FuncTable *st = malloc(sizeof *st);
        st->size = 0;
        return st;
}

int lookup_func(FuncTable* st, char* s, int args) {
        for (int i = 0; i < st->size; i++) {
                if (strcmp(st->t[i].name, s) == 0)  {
                        return i;
                }
        }
        return -1;
}

int add_func(FuncTable* st, char* s, int args, int line) {
        strcpy(st->t[st->size].name, s);
        st->t[st->size].size_args = args;
        st->t[st->size].line = line;
        int old_side = st->size;
        st->size++;
        return old_side;
}

char* get_func_name(FuncTable* st, int i) {
        return st->t[i].name;
}

int get_func_line(FuncTable* st, int i) {
        return st->t[i].line;
}

int get_func_args(FuncTable* st, int i) {
        return st->t[i].size_args;
}

void print_func_table(FuncTable* st) {
        printf("Function table:\n");
        for (int i = 0; i < st->size; i++) {
                printf("Entry %d -- name: %s, line: %d, arity: %d\n", i, get_func_name(st, i), get_func_line(st, i), get_func_args(st, i));
        }
}

void free_func_table(FuncTable* st) {
        free(st);
}


//------------------------------------------------------------------------------
// Auxtable to help passing arguments between programs - Sorry! :)

struct aux_table {
        char id[SYMBOL_MAX_SIZE][SYMBOL_TABLE_MAX_SIZE];
        int size;
};

AuxTable* create_aux_table(){
        AuxTable *st = malloc(sizeof *st);
        st->size = 0;
        return st;
}

int lookup_id(AuxTable* st, char* s){
        for (int i = 0; i < st->size; i++) {
                if (strcmp(st->id[i], s) == 0)  {
                        return i;
                }
        }
        return -1;
}

int add_id(AuxTable* st, char* s) {
        strcpy(st->id[st->size], s);
        int old_side = st->size;
        st->size++;
        return old_side;
}

char* get_id(AuxTable* st, int i) {
        return st->id[i];
}

void free_aux_table(AuxTable* st){
        free(st);
}
