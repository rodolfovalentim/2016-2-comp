
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
    BT* node = malloc(sizeof * node);
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
        case IF_NODE: sprintf(s, "%s", "if"); break;
        case THEN_NODE: sprintf(s, "%s", "then"); break;
        case ELSE_NODE: sprintf(s, "%s", "else"); break;
        case END_NODE: sprintf(s, "%s", "end"); break;
        case REPEAT_NODE: sprintf(s, "%s", "repeat"); break;
        case UNTIL_NODE: sprintf(s, "%s", "until"); break;
        case READ_NODE: sprintf(s, "%s", "read"); break;
        case WRITE_NODE: sprintf(s, "%s", "write"); break;
        case PLUS_NODE: sprintf(s, "%s", "+"); break;
        case MINUS_NODE: sprintf(s, "%s", "-"); break;
        case TIMES_NODE: sprintf(s, "%s", "*"); break;
        case OVER_NODE: sprintf(s, "%s", "/"); break;
        case EQ_NODE: sprintf(s, "%s", "="); break;
        case LT_NODE: sprintf(s, "%s", "<"); break;
        case LPAREN_NODE: sprintf(s, "%s", "("); break;
        case RPAREN_NODE: sprintf(s, "%s", ")"); break;
        case SEMI_NODE: sprintf(s, "%s", ";"); break;
        case ASSIGN_NODE: sprintf(s, "%s", ":="); break;
        case NUM_NODE: sprintf(s, "%s", node->data); break;
        case ID_NODE: sprintf(s, "%s", node->data); break;
        case EXPR_NODE: sprintf(s, "%s", "expr"); break;
        case STMT_SEQUENCE: sprintf(s, "%s", "stmt_sequence"); break;
        case IF_STMT: sprintf(s, "%s", "if_stmt"); break;
        case REPEAT_STMT: sprintf(s, "%s", "repeat_stmt"); break;
        case ASSIGN_STMT: sprintf(s, "%s", "assign_stmt"); break;
        case READ_STMT: sprintf(s, "%s", "read_stmt"); break;
        case WRITE_STMT: sprintf(s, "%s", "write_stmt"); break;
        case PROGRAM: sprintf(s, "%s", "program"); break;
        case STMT: sprintf(s, "%s", "stmt"); break;
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
