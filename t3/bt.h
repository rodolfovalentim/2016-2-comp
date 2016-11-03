#ifndef BT_H
#define BT_H

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
        STRING_NODE
} NodeKind;

struct node; // Opaque structure to ensure encapsulation.

typedef struct node BT;

BT* new_leaf(NodeKind kind, char* data);
BT* new_node(NodeKind kind, int size, ...);
BT* add_child(BT *tree, NodeKind kind, int size, ...);

void print_dot(BT *tree);
void print_tree(BT *tree);
void free_tree(BT *tree);

#endif
