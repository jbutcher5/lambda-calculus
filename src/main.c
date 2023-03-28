#include "display.h"
#include "lexer.h"
#include "parser.h"
#include "reducer.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

// true: (\\x y -> x)
// false: (\\x y -> y)

// And False False: (\\x y -> x y (\\x y -> y)) (\\x y -> y) (\\x y -> y)

int main() {
  const char *text = "(\\x y -> x y (\\x y -> y)) (\\x y -> y) (\\x y -> y)";

  int i = 0;

  LexerResult lexed = lexer(text);
  Expr expr = parser(lexed.buffer, lexed.size, text, &i);

  free(lexed.buffer);

  do {
    print_ast(expr);
  } while (beta_reduction(&expr));

  for (int i = 0; i < expr.size; i++)
    free_node(expr.ast + i);

  free(expr.ast);

  return 0;
}
