#pragma once

#include "lexer.h"

typedef enum {
  Lambda,
  NT_Ident,
  Assignment,
  Parameter,
} NodeType;

typedef struct {
  NodeType type;
  void *content;
} Node;

typedef struct {
  Node *ast;
  int size;
} Expr;

typedef struct {
  char **parameters;
  int parameter_number;
  Expr body;
} LambdaContent;

typedef struct {
  char *assignee;
  Node *body;
} AssignmentContent;

typedef struct {
  int value;
  char *name;
  LambdaContent *parent;
} ParameterContent;

Expr parser(LexerToken *tokens, int size, const char *text, int *i);
char *display_node(Node *node, char *buffer, int buffer_size);
char *display_parameters(char **parameters, int parameter_number);
void print_ast(Expr result);
void convert_de_bruijn_index(LambdaContent *lambda, int offset,
                             LambdaContent *parent);
