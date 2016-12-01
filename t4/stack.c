
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define MAX_SIZE 100

struct stack {
        int dishes[MAX_SIZE];
        int size;
};

Stack* init_stack() {
        Stack *s = malloc(sizeof *s);
        s->size = 0;
        return s;
}

int push(Stack *s, int entry) {
        return s->dishes[s->size++] = entry;
}

int pop(Stack *s) {
        return s->dishes[--s->size];
}
