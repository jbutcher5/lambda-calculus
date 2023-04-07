#include "display.h"
#include "hash.h"
#include "lexer.h"
#include "parser.h"
#include "reducer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

// true: (\\x y -> x)
// false: (\\x y -> y)

// And False False: (\\x y -> x y (\\x y -> y)) (\\x y -> y) (\\x y -> y)

int main(void) {
  const char *text = "TRUE = \\x y -> x\nFALSE = \\x y -> y\nAND = \\x y "
                     "-> x y (\\x y -> y)\n"
                     "AND FALSE TRUE\n";

  Item *table = create_table();

  int i = 0;

  LexerResult lexed = lexer(text);
  Expr expr = parser(lexed.buffer, lexed.size, text, &i, table);

  free(lexed.buffer);

  puts(text);

  replace_idents(&expr, table);

  while (beta_reduction(&expr)) {
    replace_idents(&expr, table);
    print_ast(expr);
  }

  do {
  } while (beta_reduction(&expr));

  for (int i = 0; i < expr.size; i++)
    free_node(expr.ast[i]);

  free(expr.ast);

  free_table(table);

  return 0;
}
