#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main() {

  LexerResult result = lexer("(\\x. x x) (\\x. x x)");
  LexerToken *buffer = result.buffer;

  for (int i = 0; i < result.size; i++) {
    printf("%d %d %d %d\n", i, buffer[i].type, buffer[i].start, buffer[i].end);
  }

  return 0;
  
}
