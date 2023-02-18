#pragma once

#include "lexer.h"

typedef enum {
  Lambda,
  NT_Ident,
  Assignment,
} NodeType;

typedef struct {
  NodeType type;
  void *content;
} Node;

typedef struct {
  Node *ast;
  int size;
} ParserResult;

typedef struct {
  char **parameters;
  int parameter_number;
  ParserResult body;
} LambdaContent;

typedef struct {
  int start, end;
} NT_IdentContent;

typedef struct {
  char *assignee;
  Node *body;
} AssignmentContent;

ParserResult parser(LexerToken *tokens, int size, const char *text, int *i);
char *display_node(Node *node, char *buffer, int buffer_size, const char *reference);
char *display_parameters(char **parameters, int parameter_number);