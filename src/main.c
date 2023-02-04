#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Tokens

static const char SPECIAL[] = ".\\=\n ";

// Lexer structure

typedef enum {
  Dot = 0,
  BackSlash = 1,
  Equals = 2,
  Newline = 3,
  Space = 4,
  Ident = 5
} TokenType;

typedef struct {
  TokenType type;
  int start, end;
} LexerToken;

typedef struct {
  LexerToken *buffer;
  int size;
} LexerResult;

LexerResult lexer(const char *input) {
  LexerResult result = { .buffer = (LexerToken*)calloc(sizeof(LexerToken), strlen(input)), .size = 0 };
  
  for (int i = 0; input[i]; i++) { 
    const char c = input[i];

    int token_type = 0;
    for (; SPECIAL[token_type]; token_type++)
      if (SPECIAL[token_type] == c) break;

    if (token_type != Ident) {
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

        if (next_token_type != Ident) break;  
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

int main() {

  LexerResult result = lexer("\\x.y x");
  LexerToken *buffer = result.buffer;

  for (int i = 0; i < result.size; i++) {
    printf("%d %d %d %d\n", i, buffer[i].type, buffer[i].start, buffer[i].end);
  }

  return 0;
  
}
