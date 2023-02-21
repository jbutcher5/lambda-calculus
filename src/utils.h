#pragma once

#include "lexer.h"

char *slice_string(const char *src, int start, int end);
int next_bracket(const LexerToken *tokens, int start, int size);
