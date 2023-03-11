#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "reducer.h"

int main() {
  const char *text = "(\\x -> x x) (\\x -> x x)";
  LexerResult lexed = lexer(text);
  ParserResult parsed = parser(lexed.buffer, lexed.size, text, NULL);

  do {
    print_ast(parsed);
  } while (beta_reduction(&parsed));

  return 0;
}

