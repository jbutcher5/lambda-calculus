#pragma once

#include "lexer.h"

typedef enum {
  Lambda,
  NT_Ident,
  Assignment,
} NodeType;

/*
  Lambda: LambdaContent*
  Ident: char*
  Assignment: AssignmentContent*
*/
typedef struct {
  NodeType type;
  void *content;
} Node;

typedef struct {
  char **parameters;
  Node *body;
} LambdaContent;

typedef struct {
  char *assignee;
  Node *body;
} AssignmentContent;

Node *parser(LexerResult tokens);