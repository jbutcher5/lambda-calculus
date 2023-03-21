#include "display.h"
#include "lexer.h"
#include "parser.h"
#include "reducer.h"

#include <stdio.h>
#include <stdlib.h>

// true: (\\x y -> x)
// false: (\\x y -> y)

int main() {
  const char *text = "(\\x y -> x y (\\x y -> x)) (\\x y -> x) (\\x y -> x)";

  int i = 0;

  LexerResult lexed = lexer(text);
  Expr expr = parser(lexed.buffer, lexed.size, text, &i);

  do {
    print_ast(expr);
  } while (beta_reduction(&expr));

  return 0;
}
