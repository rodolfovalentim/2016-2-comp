#ifndef TREE_H
#define TREE_H

typedef enum {
        IF_NODE,
        ELSE_NODE,
        INPUT_NODE,
        INT_NODE,
        OUTPUT_NODE,
        RETURN_NODE,
        VOID_NODE,
        WHILE_NODE,
        WRITE_NODE,
        PLUS_NODE,
        MINUS_NODE,
        TIMES_NODE,
        OVER_NODE,
        LT_NODE,
        LE_NODE,
        GT_NODE,
        GE_NODE,
        EQ_NODE,
        NEQ_NODE,
        ASSIGN_NODE,
        SEMI_NODE,
        COMMA_NODE,
        LPAREN_NODE,
        RPAREN_NODE,
        LBRACK_NODE,
        RBRACK_NODE,
        LBRACE_NODE,
        RBRACE_NODE,
        NUM_NODE,
        ID_NODE,
        STRING_NODE,
        FUNC_DECL_LIST_NODE,
        FUNC_DECL_NODE,
        FUNC_HEADER_NODE,
        FUNC_BODY_NODE,
        PARAM_LIST_NODE,
        PARAM_NODE,
        VAR_DECL_LIST_NODE,
        STMT_LIST_NODE,
        ARG_LIST_NODE,
        BLOCK_NODE,
        VAR_LIST_NODE,
        USER_FUNC_CALL_NODE
} NodeKind;

struct node; // Opaque structure to ensure encapsulation.

typedef struct node Tree;

Tree* new_node(NodeKind kind);

void add_child(Tree *parent, Tree *child);

void set_index(Tree *tree, int index);

int get_index(Tree *tree);

void set_type(Tree *tree, char* type);

char* get_type(Tree *tree);

Tree* get_child(Tree *tree, int index);

NodeKind get_kind(Tree* tree);

int get_children_size(Tree *tree);

Tree* new_subtree(NodeKind kind, int child_count, ...);

void print_tree(Tree *tree);

void print_dot(Tree *tree);

void free_tree(Tree *tree);

void node2str(Tree *node, char *s);

#endif
