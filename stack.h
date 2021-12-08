#include <stddef.h>

#ifndef STACK_H
#define STACK_H

typedef struct Node {
    double value;
    struct Node* prev;
} Node;

typedef struct Stack {
    size_t length;
    Node* top;
} Stack;

Stack* stack_create();
double stack_pop(Stack* stack);
Node* stack_node_pop(Stack* stack);
void stack_node_push(Stack* stack, Node* node);
void stack_push(Stack* stack, double val);
void stack_node_prepend(Stack* stack, Node* node, double val);
void stack_flip(Stack** stack);
void stack_to_array(Stack* stack, double* array, size_t length);
void stack_free(Stack* stack);

#endif