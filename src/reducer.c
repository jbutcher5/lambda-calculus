#include "reducer.h"
#include "parser.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_ITER_DEPTH 64

void _apply(LambdaContent *parent, LambdaContent *lambda, Node *argument) {
  for (int i = 0; i < lambda->body.size; i++) {
    Node *node = lambda->body.ast + i;

    if (node->type == Parameter) {
      if ((char *)node->content == parent->parameters[0]) {
        free_node(*node);
        *node = clone_node(*argument);
      }
    } else if (node->type == Lambda) {
      _apply(parent, node->content, argument);
    }
  }

  if (parent == lambda) {
    parent->parameter_number--;
    parent->parameters++;
  }
}

void apply(LambdaContent *lambda, Node *arguement) {
  _apply(lambda, lambda, arguement);
}

int beta_reduction(Expr *expr) {
  static Expr *last_expr = NULL;
  static int iteration = 0;

  if (!last_expr)
    last_expr = expr;

  for (int i = 0; i < expr->size; i++) {
    Node *node = expr->ast + i;
    if (node->type != NT_Expr)
      continue;
    Expr *content = node->content;

    while (beta_reduction(content))
      ;

    int new_size = expr->size + content->size - 1;
    Node *new_ast = replace_node_with_expr(expr, i, content);

    free(expr->ast);
    free(content->ast);
    free(content);

    expr->ast = new_ast;
    expr->size = new_size;

    return 1;
  }

  for (int i = 0; i < expr->size; i++) {
    Node *node = expr->ast + i;

    if (node->type == Lambda && i < expr->size - 1) {
      LambdaContent *lambda = node->content;

      apply(lambda, node + 1);
      free_node(node[1]);

      if (!lambda->parameter_number) {
        if (lambda->body.size == 2) {
          Node first = clone_node(lambda->body.ast[0]);
          Node second = clone_node(lambda->body.ast[1]);

          free_node(*node);

          node[0] = first;
          node[1] = second;
        } else {
          int new_size = expr->size - 2 + lambda->body.size;
          Node *new_ast = calloc(new_size, sizeof(Node));

          for (int j = 0; j < i; j++) {
            new_ast[j] = expr->ast[j];
          }

          for (int j = i, k = 0; k < lambda->body.size; j++, k++) {
            new_ast[j] = lambda->body.ast[k];
          }

          for (int j = i + lambda->body.size, k = i + 2; k < expr->size;
               j++, k++) {
            new_ast[j] = expr->ast[k];
          }

          LambdaContent *content = (LambdaContent *)node->content;

          char **parameters = content->parameters - (content->initial_para_num -
                                                     content->parameter_number);

          for (int i = 0; i < content->initial_para_num; i++)
            free(parameters[i]);

          free(parameters);
          free(content->body.ast);
          free(content);

          free(expr->ast);

          expr->ast = new_ast;
          expr->size = new_size;
        }
      } else {
        Node *new_ast = calloc(expr->size - 1, sizeof(Node));

        for (int j = 0; j <= i; j++) {
          new_ast[j] = expr->ast[j];
        }

        for (int j = i + 1; j < expr->size - 1; j++) {
          new_ast[j] = expr->ast[j + 1];
        }

        free(expr->ast);

        expr->ast = new_ast;
        expr->size--;
        i++;
      }

      if (last_expr == expr && iteration > MAX_ITER_DEPTH) {
        return 0;
      } else if (last_expr == expr) {
        iteration++;
      } else {
        iteration = 0;
        last_expr = expr;
      }

      return 1;
    }
  }

  return 0;
}
