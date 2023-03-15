#pragma once

#include "lexer.h"
#include "parser.h"

char *slice_string(const char *src, int start, int end);
int next_bracket(const LexerToken *tokens, int start, int size);
Node clone_node(Node node);
void free_node(Node *node);
