
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "bt.h"

struct node {
        NodeKind kind;
        char data[30];
        int size_childs;
        struct node* childs[7];
};

BT* create_node(NodeKind kind) {
        BT* node = malloc(sizeof *node);
        node->kind = kind;

        int i;
        for(i = 0; i < node->size_childs; i++) {
                node->childs[i] = NULL;
        }

        return node;
}

BT* new_leaf(NodeKind kind, char* data) {
        BT *leaf = create_node(kind);
        strcpy(leaf->data, data);
        return leaf;
}

BT* new_node(NodeKind kind, int size, ...) {
        BT *node = create_node(kind);
        node->size_childs = size;
        va_list argp;
        va_start(argp, size);
        int i;
        for(i = 0; i < node->size_childs; i++) {
                node->childs[i] = va_arg(argp, struct node*);
        }
        va_end(argp);
        return node;
}

void print_node(BT *node, int level) {
        printf("%d: Node -- Addr: %p -- Kind: %d -- Data: %s ",
               level, node, node->kind, node->data);
        int i;
        for(i = 0; i < node->size_childs; i++)
                printf("-- Child: %d: %p ", i+1, node->childs[i]);
        printf("\n");
}

void print_bt(BT *tree, int level) {
        print_node(tree, level);
        int i;
        for(i = 0; i < tree->size_childs; i++) {
                if (tree->childs[i] != NULL)
                        print_bt(tree->childs[i], level + 1);
        }
}

void print_tree(BT *tree) {
        print_bt(tree, 0);
}

void free_tree(BT *tree) {
        int i = 0;
        if (tree != NULL) {
                for(i = 0; i < tree->size_childs; i++)
                        free_tree(tree->childs[i]);
                free(tree);
        }
}

// Dot output.
void node2str(BT *node, char *s) {
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
        case EQ_NODE: sprintf(s, "%s","=" ); break;
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
        case NUM_NODE: sprintf(s, "%s","{" ); break;    //TODO Acho que tem que vir da tabela de literais ou variaveis.
        case ID_NODE: sprintf(s, "%s","{" ); break;     //TODO Acho que tem que vir da tabela de literais ou variaveis.
        case STRING_NODE: sprintf(s, "%s","{" ); break; //TODO Acho que tem que vir da tabela de literais ou variaveis.
        default: printf("Invalid node kind: %d!\n", node->kind);
        }
}
int nr;

int print_node_dot(BT *node) {
        char s[10];
        int my_nr = nr++;
        node2str(node, s);
        printf("node%d[label=\"%s\"];\n", my_nr, s);

        int i;

        for(i = 0; i < node->size_childs; i++) {
                if (node->childs[i] != NULL) {
                        int node_nr = print_node_dot(node->childs[i]);
                        printf("node%d -> node%d;\n", my_nr, node_nr);
                }
        }
        return my_nr;
}

void print_dot(BT *tree) {
        nr = 0;
        printf("digraph {\ngraph [ordering=\"out\"];\n");
        print_node_dot(tree);
        printf("}\n");
}
