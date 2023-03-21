#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"

char *display_node(Node *node, char *buffer, int buffer_size) {
  if (!buffer) {
    buffer = calloc(sizeof(char), 128);
    buffer_size = 128;
  }

  if (node->type == Lambda) {
    LambdaContent *lambda_content = (LambdaContent *)node->content;
    char *param_display = display_parameters(lambda_content->parameters,
                                             lambda_content->parameter_number);

    snprintf(buffer, buffer_size, "(\\%s->", param_display);

    for (int i = 0; i < lambda_content->body.size; i++) {
      char *node_literal = display_node(lambda_content->body.ast + i, NULL, 0);
      strcat(buffer, " ");
      strcat(buffer, node_literal);
      free(node_literal);
    }

    strcat(buffer, ")");

    free(param_display);

  } else if (node->type == NT_Ident) {
    char *ident_content = (char *)node->content;
    buffer = realloc(buffer, sizeof(char) * strlen(ident_content) + 1);
    strcpy(buffer, ident_content);
  } else if (node->type == Parameter) {
    ParameterContent *parameter_content = (ParameterContent *)node->content;
    strcpy(buffer, parameter_content->name);
  } else if (node->type == NT_Expr) {
    snprintf(buffer, buffer_size, "(%s)", display_ast(*(Expr *)node->content));
  } else {
    printf("Error: Unkown type (%d)\n", node->type);
    exit(1);
  }

  return buffer;
}

char *display_parameters(char **parameters, int parameter_number) {
  int buffer_size = sizeof(char) * 16 * parameter_number + parameter_number + 1;
  char *buffer = (char *)malloc(buffer_size);
  *buffer = 0;

  for (int i = 0; i < parameter_number; i++) {
    strcat(buffer, parameters[i]);
    strcat(buffer, " ");
  }

  return buffer;
}

char *display_ast(Expr expr) {
  char *first = display_node(expr.ast, NULL, 0);
  char *buffer = malloc((strlen(first) + 2) * sizeof(char));
  int buffer_size = strlen(first) + 2;
  strcpy(buffer, first);
  free(first);
  strcat(buffer, " ");

  for (int i = 1; i < expr.size; i++) {
    char *literal_node = display_node(expr.ast + i, NULL, 0);

    if (i + 1 == expr.size) {
      buffer_size += strlen(literal_node);
      buffer = realloc(buffer, buffer_size);
      strcat(buffer, literal_node);
    } else {
      buffer_size += strlen(literal_node) + 1;
      buffer = realloc(buffer, buffer_size);
      strcat(buffer, literal_node);
      strcat(buffer, " ");
    }

    free(literal_node);
  }

  return buffer;
}

void print_ast(Expr expr) {
  if (!expr.size)
    return;

  char *buffer = display_ast(expr);

  puts(buffer);
  free(buffer);
}
