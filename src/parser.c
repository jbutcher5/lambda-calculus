#include "parser.h"
#include "lexer.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NODE_BUFFER_DEFAULT_SIZE 16

ParserResult parser(LexerToken *tokens, int size, const char *text, int *i) {
  Node *buffer = calloc(sizeof(Node), NODE_BUFFER_DEFAULT_SIZE);
  int j = 0;
  
  while (*i < size) {
    const LexerToken token = tokens[*i];
    if (token.type == TT_Ident) {
      NT_IdentContent *content = (NT_IdentContent*)malloc(sizeof(NT_IdentContent));
      
      *content = slice_string(text, token.start, token.end+1);
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
          parameters[parameter_count] = slice_string(text, t.start, t.end+1); 
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

      convert_de_bruijn_index(content, 0, NULL);
      
      buffer[j] = (Node){Lambda, content};
      j++;
    } else if (token.type == OpenBracket) {
      int closing_bracket = next_bracket(tokens, *i, size);
      if (closing_bracket < 0) exit(1);

      (*i)++;
      ParserResult brackets = parser(tokens, closing_bracket, text, i);

      int k = 0;
      for (; k < brackets.size; k++) buffer[j] = brackets.ast[k];
      j += k;
      
    } else {
      (*i)++;
    }
  }

  return (ParserResult){buffer, j};
}

char *display_node(Node *node, char *buffer, int buffer_size) {
  if (!buffer) {
    buffer = calloc(sizeof(char), 128);
    buffer_size = 128;
  }

  if (node->type == Lambda) {
    LambdaContent *lambda_content = (LambdaContent *)node->content;
    char *param_display = display_parameters(lambda_content->parameters, lambda_content->parameter_number);
    
    snprintf(buffer, buffer_size, "(\\%s->", param_display);

    for (int i = 0; i < lambda_content->body.size; i++) {
      char *node_literal = display_node(lambda_content->body.ast+i, NULL, 0);
      strcat(buffer, " ");
      strcat(buffer, node_literal);
      free(node_literal);
    }

    strcat(buffer, ")");

    free(param_display);
    
  } else if (node->type == NT_Ident) {
    NT_IdentContent *ident_content = (NT_IdentContent *)node->content;
    buffer = realloc(buffer, sizeof(char) * strlen(*ident_content) + 1);
    strcpy(buffer, *ident_content);
  } else if (node->type == Parameter) {
    ParameterContent *parameter_content = (ParameterContent *)node->content;
    strcpy(buffer, parameter_content->name);
  } else {
    printf("Error: Unkown type (%d)\n", node->type);
    exit(1);
  }

  return buffer;
}

char *display_parameters(char **parameters, int parameter_number) {
  int buffer_size = sizeof(char) * 16 * parameter_number + parameter_number + 1;
  char *buffer = (char *)malloc(buffer_size);
  *buffer = 0;

  for (int i = 0; i < parameter_number; i++) {
    strcat(buffer, parameters[i]);
    strcat(buffer, " ");
  }

  return buffer;
}

void print_ast(ParserResult result) {
  if (!result.size) return;
  
  char *first = display_node(result.ast, NULL, 0);
  char *buffer = malloc((strlen(first) + 1) * sizeof(char));
  strcpy(buffer, first);
  free(first);
  strcat(buffer, " ");
    
  for (int i = 1; i < result.size; i++) {
    char *literal_node = display_node(result.ast + i, NULL, 0);
    
    if (i + 1 == result.size) {
      strcat(buffer, literal_node);
    } else {
      strcat(buffer, literal_node);
      strcat(buffer, " ");
    }

    free(literal_node);
  }

  puts(buffer);
  free(buffer);
}

void convert_de_bruijn_index(LambdaContent *lambda, int offset, LambdaContent *parent) {
  for (int i = 0; i < lambda->body.size; i++) {
    const Node node = lambda->body.ast[i];

    if (node.type == NT_Ident) {
      for (int j = lambda->parameter_number - 1; j >= 0; j--) {
        if (!strcmp(lambda->parameters[j], *(NT_IdentContent*)node.content)) {
          free(node.content);
          ParameterContent *content = malloc(sizeof(ParameterContent));
          content->value = j + offset;
          content->name = calloc(NODE_BUFFER_DEFAULT_SIZE, sizeof(char));
          content->parent = parent;
          
          strcpy(content->name, lambda->parameters[j]);
          lambda->body.ast[i] = (Node){Parameter, content};
        }
      }
    } else if (node.type == Lambda) {
      LambdaContent *lambda_content = (LambdaContent *)node.content;
      LambdaContent new;
  
      new = (LambdaContent){.parameters = lambda->parameters, .parameter_number = lambda->parameter_number, .body = lambda_content->body};
      convert_de_bruijn_index(&new, 0, lambda);
    
      new.parameters = lambda_content->parameters;
      new.parameter_number = lambda_content->parameter_number;
      convert_de_bruijn_index(&new, lambda->parameter_number, NULL);
    }
  }
}
