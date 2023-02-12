#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main() {

  const char *text = "\\x -> x";
  LexerResult lexed = lexer(text);
  ParserResult parsed = parser(lexed.buffer, lexed.size, text, NULL);

  return 0;
  
}
