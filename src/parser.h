#pragma once

#include "lexer.h"

typedef enum {
  Lambda,    // LambdaContent*
  NT_Ident,  // char*
  Parameter, // char*
  NT_Expr,   // Expr*
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

struct Item {
  char *key;
  Expr *value;
  struct Item *next;
};

typedef struct Item Item;

Expr parser(LexerToken *tokens, int size, const char *text, int *i,
            Item *table);
void convert_de_bruijn_index(LambdaContent *lambda, Expr *body);
