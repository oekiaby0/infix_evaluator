#include "stack.h"

#ifndef POSTFIX_H
#define POSTFIX_H

void print_postfix(Stack* postfix);
Stack* infix_to_postfix(char* str); // returns NULL if postfix format is invalid -- remember to stack_free()
double evaluate_postfix(Stack* postfix); // provided postfix should be the result from infix_to_postfix()

#endif