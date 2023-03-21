#pragma once

#include "lexer.h"

typedef enum {
  Lambda,     // LambdaContent*
  NT_Ident,   // char*
  Assignment, // void*
  Parameter,  // ParameterContent*
  NT_Expr,    // Expr*
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
void convert_de_bruijn_index(LambdaContent *lambda, int offset,
                             LambdaContent *parent);
