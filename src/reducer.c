#include "reducer.h"
#include "parser.h"

#include <stdlib.h>
#include <stdio.h>

void _apply(LambdaContent *lambda, LambdaContent *parent, Node *argument) {  
  for (int i = 0; i < lambda->body.size; i++) {
    Node *node = lambda->body.ast + i;

    if (node->type == Parameter) {
      ParameterContent *parameter = (ParameterContent*)node->content;
    
      if (!parameter->value && (parameter->parent == parent || parameter->parent == NULL)) { 
        *node = *argument;
      }
      
      else if (parameter->parent == parent) {
        parameter->value--;
      }    
    }

    else if (node->type == Lambda) {
      _apply((LambdaContent*)node->content, parent, argument);
    }
  }

  if (lambda == parent) {
    lambda->parameter_number--;
    lambda->parameters++;

    // TODO: First element in lambda->parameters is leaked here. Solve later ig
  }
}

void apply(LambdaContent *lambda, Node *argument) {
  _apply(lambda, lambda, argument);
}

void beta_reduction(ParserResult *parsed) {
  for (int i = 0; i < parsed->size; i++) {
    Node *node = parsed->ast + i;
    
    if (node->type == Lambda && i < parsed->size - 1) {
      LambdaContent *lambda = node->content;
      
      apply(node->content, node + 1);
      
      if (!lambda->parameter_number) {
        if (lambda->body.size == 2) {
          *node = lambda->body.ast[0];
          *(node+1) = lambda->body.ast[1];

          //free(lambda->parameters);
          free(lambda->body.ast);

          // TODO: Check each item of the ast is freed
        }
        else {
          int new_size = parsed->size-2+lambda->body.size;
          Node *new_ast = calloc(sizeof(Node), new_size);

          // TODO: All of these nodes we pass over need freed

          for (int j = 0; j < i; j++) {
            new_ast[j] = parsed->ast[j];
          }

          for (int j = i, k = 0; k < lambda->body.size; j++, k++) {
            new_ast[j] = lambda->body.ast[k];
          }

          for (int j = i + lambda->body.size, k = i + 2; k < parsed->size; j++, k++) {
            new_ast[j] = parsed->ast[k];
          }

          parsed->ast = new_ast;
          parsed->size = new_size;

        }
      }

      return;
    }
  }  
}