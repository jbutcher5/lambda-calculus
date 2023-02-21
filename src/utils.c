#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

char *slice_string(const char *src, int start, int end) {
  if (strlen(src) < end && start >= end && start < 0) return 0;

  int range = end - start;
  char *buffer = (char*)malloc((range + 1)*sizeof(char));

  for (int i = start; i < end; i++) buffer[i - start] = src[i];
  buffer[range] = 0;

  return buffer;
}

int next_bracket(const LexerToken *tokens, int start, int size) {
  if (!size) return -1;

  int stack = 0;
  for (int i = start; i < size; i++) {
    const LexerToken t = tokens[i];  
    
    if (t.type == OpenBracket) stack++;
    if (t.type == CloseBracket) stack--;

    if (!stack) return i;
  }

  return -1;
}

