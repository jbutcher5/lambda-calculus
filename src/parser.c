#include "parser.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NODE_BUFFER_DEFAULT_SIZE 16

ParserResult parser(LexerToken *tokens, int size, const char *text, int *i) {
  Node *buffer = calloc(sizeof(Node), NODE_BUFFER_DEFAULT_SIZE);
  int j = 0;
  
  if (!i) {
    i = (int*)malloc(sizeof(int)); 
    *i = 0;
  }
  
  while (*i < size) {
    const LexerToken token = tokens[*i];
    if (token.type == TT_Ident) {
      NT_IdentContent *content = (NT_IdentContent*)malloc(sizeof(NT_IdentContent));
      content->start = token.start;
      content->end = token.end;
      buffer[j] = (Node){NT_Ident, content};
      j++;
      (*i)++;  
    } else if (token.type == BackSlash) {
      char **parameters = (char **)malloc(sizeof(char*) * 16);
      int parameter_count = 0;

      int k = *i;
      for (; tokens[k].type != Dash && tokens[k+1].type != GreaterThan; k++) {
        if (tokens[k].type == TT_Ident) {
          const LexerToken t = tokens[k];
          parameters[parameter_count] = strndup(text+t.start, t.end-t.start); 
          parameter_count++;
        }
      }
      k += 2;
      *i = k;

      ParserResult lambda = parser(tokens, size, text, i);
      LambdaContent *content = (LambdaContent*)malloc(sizeof(LambdaContent));
      
      content->parameter_number = parameter_count;
      content->parameters = parameters;
      content->body = lambda;
      
      buffer[j] = (Node){Lambda, (void*)content};
      j++;
    } else {
      (*i)++;
    }
  }

  return (ParserResult){buffer, j};
}
