#include <stdlib.h>
#include <string.h>
#include "lexer.h"

LexerResult lexer(const char *input) {
  LexerResult result = { .buffer = (LexerToken*)calloc(sizeof(LexerToken), strlen(input)), .size = 0 };
  
  for (int i = 0; input[i]; i++) { 
    const char c = input[i];

    int token_type = 0;
    for (; SPECIAL[token_type]; token_type++)
      if (SPECIAL[token_type] == c) break;

    if (token_type != TT_Ident) {
      LexerToken t = (LexerToken){.type = token_type, .start = i, .end = i};
      result.buffer[result.size] = t;
      result.size++;
    } else {
      int j = i;
      for (; input[j]; j++) {
        const char c2 = input[j];
        
        int next_token_type = 0;
        for (; SPECIAL[next_token_type]; next_token_type++)
          if (SPECIAL[next_token_type] == c2) break;

        if (next_token_type != TT_Ident) break;  
      }
      j--;
      LexerToken token = {.type = token_type, .start = i, .end = j};      
      result.buffer[result.size] = token;
      result.size++;
      i = j;
    }
  }

  return result;
}

