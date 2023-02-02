#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFFER_INIT_SIZE 256

typedef enum {
  Dot,
  BackSlash,
  Ident
} TokenType;

typedef struct {
  TokenType type;
  int start, end;
} LexerToken;

LexerToken *parser(const char *input) {
  LexerToken current;
  LexerToken *buffer = (LexerToken*)calloc(sizeof(LexerToken), 256);
  int current_empty = 1;
  int buffer_len = 0;
  
  for (int i = 0; i < strlen(input); i++) {
    const char c = input[i];

    switch (c) {
      case '.':
        if (!current_empty) {
          buffer[buffer_len] = current;
          current_empty = 1;
          buffer_len++;
        }

        buffer[buffer_len] = (LexerToken){.type = Dot, .start = i, .end = i};
        buffer_len++;
        break;
      
      case '\\':
        if (!current_empty) {
          buffer[buffer_len] = current;
          current_empty = 1;
          buffer_len++;
        }

        buffer[buffer_len] = (LexerToken){.type = BackSlash, .start = i, .end = i};
        buffer_len++;
        break;

      case ' ':
        if (!current_empty) {
          buffer[buffer_len] = current;
          current_empty = 1;
          buffer_len++;
        }
        break;
      
      default:
        if (current_empty) {
          current = (LexerToken){.type = Ident, .start = i, .end = i};
          current_empty = 0;  
        } else {
          current.end++;
        }
        break;
    }
      
  }
  
  if (!current_empty) {
     buffer[buffer_len] = current;
     current_empty = 1;
     buffer_len++;
  }
  
  return buffer;
}

int main() {

  LexerToken *buffer = parser("\\x.y x");


  return 0;
  
}
