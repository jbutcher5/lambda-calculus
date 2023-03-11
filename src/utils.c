#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

char *slice_string(const char *src, int start, int end) {
  if (strlen(src) < end && start >= end && start < 0) return 0;

  int range = end - start;
  char *buffer = (char*)malloc((range + 1)*sizeof(char));

  for (int i = start; i < end; i++) buffer[i - start] = src[i];
  buffer[range] = 0;

  return buffer;
}

int next_bracket(const LexerToken *tokens, int start, int size) {
  if (!size) return -1;

  int stack = 0;
  for (int i = start; i < size; i++) {
    const LexerToken t = tokens[i];  
    
    if (t.type == OpenBracket) stack++;
    if (t.type == CloseBracket) stack--;

    if (!stack) return i;
  }

  return -1;
}

Node _clone_node(Node node, LambdaContent *parent) {
  Node new_node;
  
  if (node.type == Parameter) {
    ParameterContent content = *(ParameterContent*)node.content;

    char *new_str = malloc(sizeof(char)*(strlen(content.name)+1));
    strcpy(new_str, content.name);
    
    ParameterContent *new_content = malloc(sizeof(ParameterContent));
    
    new_content->value = content.value;
    new_content->parent = content.parent;
    new_content->name = new_str;

    new_node = (Node){.type = Parameter, .content = new_content};
  } else if (node.type == NT_Ident) {
    NT_IdentContent content = *(NT_IdentContent*)node.content;

    char *new_content = malloc(sizeof(char)*(strlen(content) + 1));
    strcpy(new_content, content);
  
    new_node = (Node){.type = NT_Ident, .content = new_content};
  } else if (node.type == Lambda) {
    
    LambdaContent content = *(LambdaContent*)node.content;

    LambdaContent *new_content = malloc(sizeof(LambdaContent));

    if (parent == NULL) parent = new_content;
    
    char **new_parameters = malloc(sizeof(char*)*content.parameter_number);

    for (int i = 0; i < content.parameter_number; i++) {
      new_parameters[i] = calloc(sizeof(char), strlen(content.parameters[i]));
      strcpy(new_parameters[i], content.parameters[i]);
    }

    new_content->parameters = new_parameters;
    new_content->parameter_number = content.parameter_number;

    ParserResult new_body = {.ast = malloc(sizeof(Node)*content.body.size), 
                            .size = content.body.size};

    for (int i = 0; i < content.body.size; i++) {
      Node node = content.body.ast[i];

      if (node.type == Parameter) {
        ParameterContent *content = (ParameterContent*)node.content;      
        content->parent = parent;
      }

      new_body.ast[i] = _clone_node(node, parent);
    }

    new_content->body = new_body;

    new_node = (Node){.type = Lambda, .content = new_content};
  } else {
    printf("Error: Cannot clone node of type (%d)\n", node.type); 
    exit(1);
  }

  return new_node;
}

Node clone_node(Node node) {
  return _clone_node(node, NULL);
}

void free_node(Node *node) {
  if (node->type == Lambda) {
    LambdaContent *content = (LambdaContent*)node->content;

    for (int i = 0; i < content->parameter_number; i++)
      free(content->parameters[i]);

    free(content);
    free(content->body.ast);
  } else if (node->type == NT_Ident) {
    NT_IdentContent *content = (NT_IdentContent*)node->content;

    free(*content);
    free(content);
  } else if (node->type == Parameter) {
    ParameterContent *content = (ParameterContent*)node->content;

    free(content->name);
    free(content);
  }
}