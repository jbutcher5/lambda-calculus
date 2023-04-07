#include "hash.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

int hashing_function(const char *key) {
  int length = strlen(key);

  int result = key[0];

  result |= key[1 % length] << 8;
  result |= key[length - 1] << 16;
  result |= key[(length - 2) % length] << 24;

  return result;
}

Item *create_table(void) { return (Item *)calloc(TABLE_SIZE, sizeof(Item)); }

Expr *lookup_key(Item *table, const char *str) {
  int hash = hashing_function(str);

  Item *result = table + hash % TABLE_SIZE;

  while (result->key) {
    if (!strcmp(str, result->key))
      return result->value;

    result = result->next;
  }

  return 0;
}

void insert_item(Item *table, Item item) {
  int hash = hashing_function(item.key);

  Item *lookup = table + hash % TABLE_SIZE;

  while (1) {
    if (!lookup->key) {
      *lookup = item;

      if (!lookup->next)
        lookup->next = calloc(1, sizeof(Item));

      break;
    }

    lookup = lookup->next;
  }
}

void free_table(Item *table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    Item *item = table + i;

    if (item->key && item->value) {
      free(item->key);
      free_node((Node){.type = NT_Expr, .content = item->value});
    }

    item = item->next;

    while (item) {
      Item *next = item->next;

      if (item->key && item->value) {
        free(item->key);
        free_node((Node){.type = NT_Expr, .content = item->value});
      }

      free(item);

      item = next;
    }
  }

  free(table);
}
