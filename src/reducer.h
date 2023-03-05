#pragma once

#include "parser.h"

void apply(LambdaContent *lambda, Node *node);
int beta_reduction(ParserResult *parsed);
