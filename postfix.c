#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "postfix.h"

#define whitespace(c) (c == 32 || (c >= 9 && c <= 13)) 
#define number(c) (c >= '0' && c <= '9')

#define uchar unsigned char


enum Tokens {
    CLOSE_BRACKET_TOKEN=1,
    ADDITION_TOKEN,
    SUBTRACTION_TOKEN,
    MULTIPLICATION_TOKEN,
    DIVISION_TOKEN,
    POWER_TOKEN,
    OPEN_BRACKET_TOKEN
};


void postfix_print(Node* node) {
    if (node->prev) {
        postfix_print(node->prev);
    }
    if (node->value < 0) {
        char c;
        switch ((long) -node->value) {
            case CLOSE_BRACKET_TOKEN: c = ')'; break;
            case ADDITION_TOKEN: c = '+'; break;
            case SUBTRACTION_TOKEN: c = '-'; break;
            case MULTIPLICATION_TOKEN: c = '*'; break;
            case DIVISION_TOKEN: c = '/'; break;
            case POWER_TOKEN: c = '^'; break;
            case OPEN_BRACKET_TOKEN: c = '('; break;
            default: {
                c = '?';
            }
        }
        if (c) printf("%c ", c);
    } else {
        printf("%.16g ", node->value);
    }
}

void print_postfix(Stack* postfix) {
    postfix_print(postfix->top);
    printf("\n");
}

uchar operator(char c) {
    switch (c) {
        case ')': return CLOSE_BRACKET_TOKEN;
        case '+': return ADDITION_TOKEN;
        case '-': return SUBTRACTION_TOKEN;
        case '*': return MULTIPLICATION_TOKEN;
        case '/': return DIVISION_TOKEN;
        case '^': return POWER_TOKEN;
        case '(': return OPEN_BRACKET_TOKEN;
    }
    return 0;
}

struct NumberInfo {
    size_t length;
    size_t decimal;
};

struct NumberInfo* get_number_info(char* c) {
    struct NumberInfo* info = malloc(sizeof(struct NumberInfo));
    info->length = 0;
    while (*c && (number(*c) || *c == '.') ) {
        if (*c == '.') {
            if (info->decimal) {
                free(info);
                return NULL;
            }
            info->decimal = info->length;
        }
        info->length++;
        c++;
    }
    if (
        (*c && !operator(*c) && !whitespace(*c)) ||
        (*--c == '.')
    ) {
        free(info);
        return NULL;
    }
    return info;
}

double get_number(char* c, size_t len, size_t decimal) {
    if (len == 1) return *c - '0';
    double result = 0;
    if (decimal) {
        do {
            uchar digit = *c++ - '0';
            --decimal;
            if (digit != 0) result += digit * pow(10, decimal);
            --len;
        } while (*c != '.' && decimal > 0);
        c++;
        size_t i = 0;
        double right = 0;
        len--;
        do {
            uchar digit = *c++ - '0';
            --len;
            if (digit != 0) right += digit * pow(10, len);
            i++;
        } while (len > 0);
        return result + (right / pow(10, i));
    } else {
        do {
            uchar digit = *c++ - '0';
            --len;
            if (digit != 0) result += digit * pow(10, len);
        } while (len > 0);
        return result;
    }
}

Stack* tokenize_infix(char* c) {
    Stack* tokens = stack_create();
    while (*c) {
        uchar op = operator(*c);
        if (op) {
            stack_push(tokens, -op);
            c++;
        } else if (number(*c)) {
            struct NumberInfo* info = get_number_info(c);
            if (info == NULL) {
                stack_free(tokens);
                return NULL;
            }
            stack_push(tokens, get_number(c, info->length, info->decimal));
            c += info->length;
            free(info);
        } else if (whitespace(*c)) {
            c++;
        } else {
            stack_free(tokens);
            return NULL;
        }
    }
    return tokens;
}

uchar check_infix_grammar(Stack* tokens) {
    if (tokens->top->value < -1) {
        return 0;
    }
    if (!tokens->top->prev) return 1;
    Node* node = tokens->top;
    size_t bracket_count = 0;
    do {
        if (node->value == -OPEN_BRACKET_TOKEN) bracket_count++;
        else if (node->value == -CLOSE_BRACKET_TOKEN) bracket_count--;

        if (
            (node->prev->value == -CLOSE_BRACKET_TOKEN && (node->value >= 0 || node->value == -OPEN_BRACKET_TOKEN)) ||
            (node->value == -OPEN_BRACKET_TOKEN && node->prev->value >= 0)
        ) {
            if (bracket_count == 1) {
                return 0;
            }
        /*
            3(5+1) -> 3 * (5+1)
            (5+1)3 -> (5+1) * 3
            (3+1)(5+1) -> (3+1) * (5+1)
        */
            stack_node_prepend(tokens, node, -MULTIPLICATION_TOKEN);
            continue;
        }
        if ( 
            (node->prev->value < 0) == (node->value < 0) 
            && node->value != -OPEN_BRACKET_TOKEN && node->prev->value != -CLOSE_BRACKET_TOKEN
        ) {
            return 0;
        }
    } while ((node = node->prev)->prev);

    if (node->value == -OPEN_BRACKET_TOKEN) bracket_count++;
    else if (node->value == -CLOSE_BRACKET_TOKEN) bracket_count--;
    if (bracket_count != 0) {
        return 0;
    }
    if (node->value < 0 && node->value != -OPEN_BRACKET_TOKEN) {
        return 0;
    }
    return 1;
}

Stack* infix_to_postfix(char* str) {
    Stack* token_stack = tokenize_infix(str);
    if (token_stack == NULL) return NULL;
    else if (!check_infix_grammar(token_stack)) {
        stack_free(token_stack);
        return NULL;
    }

    size_t tokens_length = token_stack->length;
    double* tokens = malloc(sizeof(double) * tokens_length);
    stack_to_array(token_stack, tokens, tokens_length);
    stack_free(token_stack);

    Stack* operators = stack_create();
    Stack* postfix = stack_create();

    for (size_t i=0; i < tokens_length; i++) {
        double c = tokens[i];
        if (c >= 0) {
            stack_push(postfix, c);
        } else if (c == -OPEN_BRACKET_TOKEN) {
            stack_push(operators, c);
        } else if (c == -CLOSE_BRACKET_TOKEN) {
            while (operators->top && operators->top->value != -OPEN_BRACKET_TOKEN) {
                stack_push(postfix, stack_pop(operators));
            }
            stack_pop(operators);
        } else {
            if (operators->top) {
                while (
                    operators->top && -operators->top->value > -c && 
                    operators->top->value != -OPEN_BRACKET_TOKEN // && operators->top->value != -CLOSE_BRACKET
                ) {
                    stack_push(postfix, stack_pop(operators));
                }
            }
            stack_push(operators, c);
        }    
    }
    
    while (operators->top) {
        stack_push(postfix, stack_pop(operators));
    }
    stack_free(operators);

    if (postfix->top) return postfix;
    stack_free(postfix);
    return NULL;
}

double evaluate_postfix(Stack* postfix_stack) {
    size_t length = postfix_stack->length;
    double* postfix = malloc(sizeof(double) * length);
    stack_to_array(postfix_stack, postfix, length);
    Stack* operands = stack_create();
    for (size_t i=0; i < length; i++) {
        double token = postfix[i];
        if (token >= 0) {
            stack_push(operands, token);
        } else {
            double val1 = stack_pop(operands);
            double val2 = stack_pop(operands);
            switch ((uchar) -token) {
                case ADDITION_TOKEN: stack_push(operands, val2 + val1); break;
                case SUBTRACTION_TOKEN: stack_push(operands, val2 - val1); break;
                case MULTIPLICATION_TOKEN: stack_push(operands, val2 * val1); break;
                case DIVISION_TOKEN: stack_push(operands, val2/val1); break;
                case POWER_TOKEN: stack_push(operands, pow(val2, val1)); break;
            }
        }
    }
    double result = stack_pop(operands);
    stack_free(operands);
    return result;
}