#include "lexer.h"
#include "parser.h"
#include "reducer.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  const char *text = "(\\x -> x) ((\\y -> y y) (\\z -> z z))";

  int i = 0;

  LexerResult lexed = lexer(text);
  Expr expr = parser(lexed.buffer, lexed.size, text, &i);

  do {
    print_ast(expr);
  } while (beta_reduction(&expr));

  return 0;
}
