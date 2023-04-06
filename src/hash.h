#pragma once

#include "parser.h"

#define TABLE_SIZE 512

int hashing_function(const char *key);

Item *create_table(void);
Expr *lookup_key(Item *table, const char *str);
void insert_item(Item *table, Item item);
void free_table(Item *table);
