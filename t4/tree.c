
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

#define MAX_TYPE_SIZE 6
#define TEXT_LIMIT 256
int CHILDREN_LIMIT = 100;

struct node {
        NodeKind kind;
        char type[MAX_TYPE_SIZE];
        int index;
        int count;
        Tree** child;
};

Tree* new_node(NodeKind kind) {
        Tree* node = malloc(sizeof *node);
        node->kind = kind;
        node->count = 0;
        node->index = 0;
        node->child = (Tree**) malloc(CHILDREN_LIMIT*sizeof(Tree*));
        for (int i = 0; i < CHILDREN_LIMIT; i++) {
                node->child[i] = NULL;
        }
        return node;
}

void add_child(Tree *parent, Tree *child) {
        if (parent->count == CHILDREN_LIMIT) {
                Tree** tmp;
                tmp = (Tree**)realloc(parent->child, (CHILDREN_LIMIT+=100)*sizeof(Tree*));
                parent->child = tmp;
        }
        parent->child[parent->count] = child;
        parent->count++;
}

void set_type(Tree *tree, char* type){
        strcpy(tree->type, type);
}

char* get_type(Tree *tree){
        return tree->type;
}

void set_index(Tree *tree, int index){
        tree->index = index;
}

int get_index(Tree *tree){
        return tree->index;
}

NodeKind get_kind(Tree* tree){
        return tree->kind;
}

Tree* get_child(Tree *tree, int index){
        return tree->child[index];
}

int get_children_size(Tree *tree){
        return tree->count;
}

Tree* new_subtree(NodeKind kind, int child_count, ...) {

        Tree* node = new_node(kind);

        if (child_count > CHILDREN_LIMIT) {
                Tree** tmp;
                // fprintf(stderr, "Reallocing to fit children!\n");
                tmp = (Tree**)realloc(node->child, (CHILDREN_LIMIT += 100)*sizeof(Tree*));
                node->child = tmp;
        }

        va_list ap;
        va_start(ap, child_count);
        for (int i = 0; i < child_count; i++) {
                add_child(node, va_arg(ap, Tree*));
        }
        va_end(ap);
        return node;
}

void print_node(Tree *node, int level) {
        char s[TEXT_LIMIT];
        node2str(node, s);
        printf("%d: Node -- Addr: %p -- Text: %s -- Count: %d\n",
               level, node, s, node->count);
        for (int i = 0; i < node->count; i++) {
                print_node(node->child[i], level+1);
        }
}

void print_tree(Tree *tree) {
        print_node(tree, 0);
}

void free_tree(Tree *tree) {
        for (int i = 0; i < tree->count; i++) {
                free_tree(tree->child[i]);
        }
        free(tree);
}

void node2str(Tree *node, char *s) {
        switch(node->kind) {
        case IF_NODE: sprintf(s, "%s","if" ); break;
        case ELSE_NODE: sprintf(s, "%s","else" ); break;
        case INPUT_NODE: sprintf(s, "%s","input" ); break;
        case INT_NODE: sprintf(s, "%s","int" ); break;
        case OUTPUT_NODE: sprintf(s, "%s","output" ); break;
        case RETURN_NODE: sprintf(s, "%s","return" ); break;
        case VOID_NODE: sprintf(s, "%s","void" ); break;
        case WHILE_NODE: sprintf(s, "%s","while" ); break;
        case WRITE_NODE: sprintf(s, "%s","write" ); break;
        case PLUS_NODE: sprintf(s, "%s","+" ); break;
        case MINUS_NODE: sprintf(s, "%s","-" ); break;
        case TIMES_NODE: sprintf(s, "%s","*" ); break;
        case OVER_NODE: sprintf(s, "%s","/" ); break;
        case LT_NODE: sprintf(s, "%s","<" ); break;
        case LE_NODE: sprintf(s, "%s","<=" ); break;
        case GT_NODE: sprintf(s, "%s",">" ); break;
        case GE_NODE: sprintf(s, "%s",">=" ); break;
        case EQ_NODE: sprintf(s, "%s","==" ); break;
        case NEQ_NODE: sprintf(s, "%s","!=" ); break;
        case ASSIGN_NODE: sprintf(s, "%s","=" ); break;
        case SEMI_NODE: sprintf(s, "%s",";" ); break;
        case COMMA_NODE: sprintf(s, "%s","," ); break;
        case LPAREN_NODE: sprintf(s, "%s","(" ); break;
        case RPAREN_NODE: sprintf(s, "%s",")" ); break;
        case LBRACK_NODE: sprintf(s, "%s","[" ); break;
        case RBRACK_NODE: sprintf(s, "%s","]" ); break;
        case LBRACE_NODE: sprintf(s, "%s","}" ); break;
        case RBRACE_NODE: sprintf(s, "%s","{" ); break;
        case NUM_NODE: sprintf(s, "%s,%d", node->type, node->index); break;
        case ID_NODE: sprintf(s, "%s,%d", node->type, node->index); break;
        case STRING_NODE: sprintf(s, "%s,%d", node->type, node->index); break;
        case FUNC_DECL_LIST_NODE: sprintf(s, "%s", "func_list"); break;
        case FUNC_DECL_NODE: sprintf(s, "%s", "func_decl"); break;
        case FUNC_HEADER_NODE: sprintf(s, "%s", "func_header"); break;
        case FUNC_BODY_NODE: sprintf(s, "%s", "func_body"); break;
        case PARAM_LIST_NODE: sprintf(s, "%s", "param_list"); break;
        case PARAM_NODE: sprintf(s, "%s", "param"); break;
        case VAR_DECL_LIST_NODE: sprintf(s, "%s", "var_list"); break;
        case STMT_LIST_NODE: sprintf(s, "%s", "stmt_list"); break;
        case ARG_LIST_NODE: sprintf(s, "%s", "arg_list"); break;
        case BLOCK_NODE: sprintf(s, "%s", "block"); break;
        case USER_FUNC_CALL_NODE: sprintf(s, "%s,%d", "fcall", node->index); break;
        default: printf("Invalid node kind: %d!\n", node->kind);
        }
}

// Dot output.
int nr;

int print_node_dot(Tree *node) {
        int my_nr = nr++;
        char s[TEXT_LIMIT];
        node2str(node, s);
        printf("node%d[label=\"%s\"];\n", my_nr, s);
        for (int i = 0; i < node->count; i++) {
                int child_nr = print_node_dot(node->child[i]);
                printf("node%d -> node%d;\n", my_nr, child_nr);
        }
        return my_nr;
}

void print_dot(Tree *tree) {
        nr = 0;
        printf("digraph {\ngraph [ordering=\"out\"];\n");
        print_node_dot(tree);
        printf("}\n");
}
