#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main() {

  const char *text = "\\x -> x";
  LexerResult tokens = lexer(text);
  ParserResult ast = parser(tokens.buffer, tokens.size, text, NULL);

  return 0;
  
}
