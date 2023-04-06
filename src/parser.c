#include "parser.h"
#include "hash.h"
#include "lexer.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE_BUFFER_DEFAULT_SIZE 16

Expr parser(LexerToken *tokens, int size, const char *text, int *i,
            Item *table) {
  Node *buffer = calloc(NODE_BUFFER_DEFAULT_SIZE, sizeof(Node));
  int j = 0;

  LexerToken token;

  int offset;

  while (*i < size) {
    token = tokens[*i];

    if (token.type == TT_Ident && (*i == 0 || tokens[*i - 1].type == Newline) &&
        (offset =
             ignore_space_find_token(tokens + *i, size - *i - 1, 0, Equals))) {
      char *key = slice_string(text, token.start, token.end + 1);

      *i += offset + ignore_space_find_token(tokens + *i + offset,
                                             size - (*i + offset) - 1, 1, 0);

      int next_end_line =
          ignore_space_find_token(tokens + *i, size - *i, 0, Newline);

      if (!next_end_line)
        next_end_line = size;
      else
        next_end_line += *i;

      Expr expr = parser(tokens, next_end_line, text, i, table);

      Expr *value = calloc(1, sizeof(Expr));
      *value = expr;

      Item item = {.key = key, .value = value};

      insert_item(table, item);

      *i = next_end_line + 1;
    } else if (token.type == TT_Ident) {
      char *content = slice_string(text, token.start, token.end + 1);
      buffer[j] = (Node){NT_Ident, content};

      j++;
      (*i)++;
    } else if (token.type == BackSlash) {
      char **parameters = malloc(sizeof(char *) * 16);
      int parameter_count = 0;

      int k = *i;
      for (; tokens[k].type != Dash && tokens[k + 1].type != GreaterThan; k++) {
        if (tokens[k].type == TT_Ident) {
          const LexerToken t = tokens[k];
          parameters[parameter_count] = slice_string(text, t.start, t.end + 1);
          parameter_count++;
        }
      }

      k += 2;
      *i = k;

      Expr lambda = parser(tokens, size, text, i, table);
      LambdaContent *content = malloc(sizeof(LambdaContent));

      content->parameter_number = parameter_count;
      content->parameters = parameters;
      content->body = lambda;
      content->initial_para_num = parameter_count;

      convert_de_bruijn_index(content, &content->body);

      buffer[j] = (Node){Lambda, content};
      j++;
    } else if (token.type == OpenBracket) {
      int closing_bracket = next_bracket(tokens, *i, size);
      if (closing_bracket < 0)
        exit(1);

      (*i)++;

      Expr brackets = parser(tokens, closing_bracket, text, i, table);

      if (brackets.size == 1) {
        buffer[j] = brackets.ast[0];
        j++;

        free(brackets.ast);
      } else if (brackets.size > 1) {
        Expr *content = malloc(sizeof(Expr));
        *content = brackets;

        buffer[j] = (Node){.type = NT_Expr, .content = content};
        j++;
      }
    } else {
      (*i)++;
    }
  }

  return (Expr){buffer, j};
}
