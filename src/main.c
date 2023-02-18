#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main() {
  const char *text = "(\\x -> x) (\\x -> x)";
  LexerResult lexed = lexer(text);
  ParserResult parsed = parser(lexed.buffer, lexed.size, text, NULL);

  printf("%s", display_node(parsed.ast, NULL, 0, text));

  return 0;
}
