#pragma once

#include "parser.h"

char *display_node(Node *node, char *buffer, int buffer_size);
char *display_parameters(char **parameters, int parameter_number);
char *display_ast(Expr expr);
void print_ast(Expr expr);
