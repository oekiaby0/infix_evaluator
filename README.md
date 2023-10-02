# Infix Expression Evaluator
This C code provides functions to evaluate infix expressions. It accomplishes this by first converting the infix expressions into postfix notation (Reverse Polish Notation or RPN) and then evaluating the postfix expressions.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Functions](#functions)
- [Example](#example)
- [License](#license)

## Prerequisites
Before using this code, you need:

- A C compiler to build and run the code.
- Basic understanding of infix and postfix notations.

## Usage
1. Include the necessary header files at the beginning of your C program:

```c
#include "postfix.h"
```
2. Define your infix expression as a C string.
3. Use the provided functions to tokenize the infix expression, convert it to postfix notation, and evaluate it.

## Functions
### Stack* infix_to_postfix(char* str)
This function takes an infix expression as input and returns a pointer to a Stack structure containing the corresponding postfix expression. If the input is invalid or contains syntax errors, it returns NULL.

### double evaluate_postfix(Stack* postfix_stack)
This function evaluates a postfix expression represented as a Stack of doubles. It returns the result of the expression evaluation.

### void print_postfix(Stack* postfix)
This function prints a postfix expression from a Stack.

## Example
Here's an example of how to use these functions:

```c
#include <stdio.h>
#include "postfix.h"

int main() {
    char infix_expression[] = "3 + (5 * 2)";  // Replace with your infix expression

    Stack* postfix_stack = infix_to_postfix(infix_expression);

    if (postfix_stack != NULL) {
        printf("Postfix Expression: ");
        print_postfix(postfix_stack);

        double result = evaluate_postfix(postfix_stack);
        printf("Result: %.2f\n", result);

        stack_free(postfix_stack);
    } else {
        printf("Invalid infix expression.\n");
    }

    return 0;
}
```

### License
This code is provided under the GPL3 License. Feel free to use, modify, and distribute it as needed. See the LICENSE file for more details.
