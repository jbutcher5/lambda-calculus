#pragma once

static const char SPECIAL[] = ".\\=()\n ";

typedef enum {
  Dot,
  BackSlash,
  Equals,
  OpenBracket,
  CloseBracket,
  Newline,
  Space,
  TT_Ident,
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
 