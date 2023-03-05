#pragma once

#include "parser.h"

void apply(LambdaContent *lambda, Node *node);
void beta_reduction(ParserResult *parsed);
