#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "reducer.h"

int main() {
  const char *text = "(\\x -> x x) a";

  int i = 0;
  
  LexerResult lexed = lexer(text);
  ParserResult parsed = parser(lexed.buffer, lexed.size, text, &i);

  do {
    print_ast(parsed);
  } while (beta_reduction(&parsed));

  return 0;
}

