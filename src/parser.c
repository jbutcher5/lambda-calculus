#include "parser.h"
#include "lexer.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE_BUFFER_DEFAULT_SIZE 16

Expr parser(LexerToken *tokens, int size, const char *text, int *i) {
  Node *buffer = calloc(NODE_BUFFER_DEFAULT_SIZE, sizeof(Node));
  int j = 0;

  while (*i < size) {
    const LexerToken token = tokens[*i];
    if (token.type == TT_Ident) {
      char *content = slice_string(text, token.start, token.end + 1);
      buffer[j] = (Node){NT_Ident, content};

      j++;
      (*i)++;
    } else if (token.type == BackSlash) {
      char **parameters = (char **)malloc(sizeof(char *) * 16);
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

      Expr lambda = parser(tokens, size, text, i);
      LambdaContent *content = (LambdaContent *)malloc(sizeof(LambdaContent));

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

      Expr brackets = parser(tokens, closing_bracket, text, i);

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
