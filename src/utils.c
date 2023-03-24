#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

char *slice_string(const char *src, int start, int end) {
  if (strlen(src) < end && start >= end && start < 0)
    return 0;

  int range = end - start;
  char *buffer = (char *)malloc((range + 1) * sizeof(char));

  for (int i = start; i < end; i++)
    buffer[i - start] = src[i];
  buffer[range] = 0;

  return buffer;
}

int next_bracket(const LexerToken *tokens, int start, int size) {
  if (!size)
    return -1;

  int stack = 0;
  for (int i = start; i < size; i++) {
    const LexerToken t = tokens[i];

    if (t.type == OpenBracket)
      stack++;
    if (t.type == CloseBracket)
      stack--;

    if (!stack)
      return i;
  }

  return -1;
}

Node clone_node(Node node) {
  Node new_node;

  // TODO: Add clone for NT_Expr

  if (node.type == Parameter) {
    new_node = (Node){.type = Parameter, .content = (char *)node.content};
  } else if (node.type == NT_Ident) {
    char *content = node.content;

    char *new_content = calloc((strlen(content) + 1), sizeof(char));
    strcpy(new_content, content);

    new_node = (Node){.type = NT_Ident, .content = new_content};
  } else if (node.type == Lambda) {
    LambdaContent content = *(LambdaContent *)node.content;

    LambdaContent *new_content = malloc(sizeof(LambdaContent));

    char **new_parameters = malloc(sizeof(char *) * content.parameter_number);

    for (int i = 0; i < content.parameter_number; i++) {
      new_parameters[i] =
          calloc(strlen(content.parameters[i]) + 1, sizeof(char));
      strcpy(new_parameters[i], content.parameters[i]);
    }

    new_content->parameters = new_parameters;
    new_content->parameter_number = content.parameter_number;
    new_content->initial_para_num = content.initial_para_num;

    Expr new_body = {.ast = malloc(sizeof(Node) * content.body.size),
                     .size = content.body.size};

    for (int i = 0; i < content.body.size; i++) {
      Node node = content.body.ast[i];

      if (node.type == Parameter) {
        char *name = calloc(strlen(node.content) + 1, sizeof(char));
        strcpy(name, node.content);

        new_body.ast[i] = (Node){.type = NT_Ident, .content = name};
      } else {
        new_body.ast[i] = clone_node(node);
      }
    }

    new_content->body = new_body;

    new_node = (Node){.type = Lambda, .content = new_content};
    convert_de_bruijn_index(new_content, &new_content->body);
  } else if (node.type == NT_Expr) {
    Expr content = *(Expr *)node.content;

    Expr *new_content = malloc(sizeof(Expr));
    Node *new_ast = calloc(content.size, sizeof(Node));

    for (int i = 0; i < content.size; i++)
      new_ast[i] = clone_node(content.ast[i]);

    new_content->size = content.size;
    new_content->ast = new_ast;

    new_node = (Node){.type = NT_Expr, .content = new_content};
  } else {
    printf("Error: Cannot clone node of type %d\n", node.type);
    exit(1);
  }

  return new_node;
}

void free_node(Node *node) {
  if (node->type == Lambda) {
    LambdaContent *content = (LambdaContent *)node->content;

    for (int i = 0; i < content->parameter_number; i++)
      free(content->parameters[i]);

    free(content->parameters -
         (content->initial_para_num - content->parameter_number));
    free(content->body.ast);
    free(content);
  } else if (node->type == NT_Ident) {
    char *content = node->content;

    free(content);
  } else if (node->type == Parameter) {
    // Nothing to do here content is dependent on pointer to parameter and
    // lambda should always outlive this node

  } else if (node->type == NT_Expr) {
    Expr *content = (Expr *)node->content;

    for (int i = 0; i < content->size; i++)
      free_node(content->ast + i);

    free(content->ast);
    free(content);
  } else {
    printf("Error: Cannot free node of type %d\n", node->type);
    exit(1);
  }
}

void convert_de_bruijn_index(LambdaContent *lambda, Expr *body) {
  for (int i = 0; i < body->size; i++) {
    Node *node = body->ast + i;

    if (node->type == NT_Ident) {
      for (int j = 0; j < lambda->parameter_number; j++) {
        if (!strcmp((char *)node->content, lambda->parameters[j])) {
          free(node->content);

          *node = (Node){.type = Parameter, .content = lambda->parameters[j]};
          break;
        }
      }
    } else if (node->type == Lambda) {
      LambdaContent *content = node->content;

      convert_de_bruijn_index(lambda, &content->body);
      convert_de_bruijn_index(content, &content->body);
    } else if (node->type == Parameter) {
      for (int j = 0; j < lambda->parameter_number; j++) {
        if (!strcmp((char *)node->content, lambda->parameters[j])) {
          *node = (Node){.type = Parameter, .content = lambda->parameters[j]};
          break;
        }
      }
    }
  }
}
