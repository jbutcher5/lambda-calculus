#pragma once

#include "lexer.h"

typedef enum {
  Lambda,     // LambdaContent*
  NT_Ident,   // char*
  Assignment, // void*
  Parameter,  // char*
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
  int initial_para_num;
  Expr body;
} LambdaContent;

typedef struct {
  char *assignee;
  Node *body;
} AssignmentContent;

Expr parser(LexerToken *tokens, int size, const char *text, int *i);
void convert_de_bruijn_index(LambdaContent *lambda, Expr *body);
