
#ifndef STACK_H
#define STACK_H

typedef struct stack Stack;

Stack* init_stack();

int push(Stack *s, int entry);

int pop(Stack *s);

#endif
