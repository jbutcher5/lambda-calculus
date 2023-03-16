#include "lexer.h"
#include "parser.h"
#include "reducer.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  const char *text = "(\\y -> y y) (\\z -> z z)";

  int i = 0;

  LexerResult lexed = lexer(text);
  ParserResult parsed = parser(lexed.buffer, lexed.size, text, &i);

  do {
    print_ast(parsed);
  } while (beta_reduction(&parsed));

  return 0;
}
