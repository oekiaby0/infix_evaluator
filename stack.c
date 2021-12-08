#include "stack.h"
#include <stdlib.h>

void node_free(Node* node) {
    if (node->prev) {
        node_free(node->prev);
    }
    free(node);
}

Stack* stack_create() {
    Stack* stack = malloc(sizeof(Stack));
    stack->length = 0;
    return stack;
}

double stack_pop(Stack* stack) {
    double val = stack->top->value;
    Node* old = stack->top;
    stack->top = old->prev;
    free(old);
    stack->length--;
    return val;
}

Node* stack_node_pop(Stack* stack) {
    Node* old = stack->top;
    stack->top = old->prev;
    stack->length--;
    return old;
}

void stack_node_push(Stack* stack, Node* node) {
    node->prev = stack->top;
    stack->top = node;
    stack->length++;
}

void stack_push(Stack* stack, double val) {
    Node* node = malloc(sizeof(Node));
    node->value = val;
    node->prev = stack->top;
    stack->top = node;
    stack->length++;
}

void stack_node_prepend(Stack* stack, Node* node, double val) {
    Node* new = malloc(sizeof(Node));
    new->value = val;
    new->prev = node->prev;
    node->prev = new;
    stack->length++;
}

void stack_flip(Stack** stack) {
    Stack* flipped = malloc(sizeof(Stack));
    while ((*stack)->top) stack_node_push(flipped, stack_node_pop(*stack));
    free(*stack);
    *stack = flipped;
}

void stack_to_array(Stack* stack, double* array, size_t len) {
    while (len-- > 0) {
        array[len] = stack_pop(stack);
    }
}

void stack_free(Stack* stack) {
    if (stack->top) {
        node_free(stack->top);
    }
    free(stack);
}