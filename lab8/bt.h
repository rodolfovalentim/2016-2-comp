#ifndef BT_H
#define BT_H

typedef enum {
  IF_NODE,
  THEN_NODE,
  ELSE_NODE,
  END_NODE,
  REPEAT_NODE,
  UNTIL_NODE,
  READ_NODE,
  WRITE_NODE,
  PLUS_NODE,
  MINUS_NODE,
  TIMES_NODE,
  OVER_NODE,
  EQ_NODE,
  LT_NODE,
  LPAREN_NODE,
  RPAREN_NODE,
  SEMI_NODE,
  ASSIGN_NODE,
  NUM_NODE,
  ID_NODE,
  EXPR_NODE,
  STMT_SEQUENCE,
  IF_STMT,
  REPEAT_STMT,
  ASSIGN_STMT,
  READ_STMT,
  WRITE_STMT,
  PROGRAM,
  STMT,
} NodeKind;

struct node; // Opaque structure to ensure encapsulation.

typedef struct node BT;

BT* new_leaf(NodeKind kind, char* data);
BT* new_node(NodeKind kind, int size, ...);

void print_dot(BT *tree);
void print_tree(BT *tree);
void free_tree(BT *tree);

#endif
