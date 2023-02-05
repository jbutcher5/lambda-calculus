#pragma once

static const char SPECIAL[] = ".\\=()\n ";

typedef enum {
  Dot = 0,
  BackSlash = 1,
  Equals = 2,
  OpenBracket = 3,
  CloseBracket  = 4,
  Newline = 5,
  Space = 6,
  Ident = 7
} TokenType;

typedef struct {
  TokenType type;
  int start, end;
} LexerToken;

typedef struct {
  LexerToken *buffer;
  int size;
} LexerResult;

LexerResult lexer(const char *input);
 