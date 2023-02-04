#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFFER_INIT_SIZE 256

// Character significance in grammar

static const char SPECIAL[] = ".\\=\n";
static const char IGNORE[] = " ";

// Lexer structure

typedef enum {
  Dot = 0,
  BackSlash = 1,
  Equals = 2,
  Newline = 3,
  End = 4,
  Ident = 5
} TokenType;

typedef struct {
  TokenType type;
  int start, end;
} LexerToken;

LexerToken *lexer(const char *input) {
  LexerToken current;
  LexerToken *buffer = (LexerToken*)calloc(sizeof(LexerToken), TOKEN_BUFFER_INIT_SIZE);
  int current_empty = 1;
  int buffer_len = 0;

  for (int i = 0; i < strlen(input) + 1; i++) { 
    const char c = input[i];

    // Check if c is in ignore list
    
    int j = 0;
    for (; j < strlen(IGNORE); j++)
      if (IGNORE[j] == c) break;
    if (j < strlen(IGNORE)) continue;

    // Check is c is in special list

    int token_type = 0;
    for (; token_type < strlen(SPECIAL) + 1; token_type++)
      if (SPECIAL[token_type] == c) break;

    if (token_type != Ident && !current_empty) {
      buffer[buffer_len] = current;
      current_empty = 1;
      buffer_len++;

      buffer[buffer_len] = (LexerToken){.type = token_type, .start = i, .end = i};
      buffer_len++;
    } else if (token_type < Ident && current_empty) {
      buffer[buffer_len] = (LexerToken){.type = token_type, .start = i, .end = i};
      buffer_len++;
    } else if (token_type == Ident && current_empty) {
      current = (LexerToken){.type = Ident, .start = i, .end = i};
      current_empty = 0;
    } else {
      current.end++;
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

  LexerToken *buffer = lexer("\\x.y x");

  return 0;
  
}
