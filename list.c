#include "list.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

List* newList(List* parent)
{
    List* list = calloc(1, sizeof(List));

    if (!list) {
        fprintf(stderr, "bad alloc");
        exit(EXIT_FAILURE);
    }
    list->left = parent;
    return list;
}

void push_back(List* list, char* string)
{
    if (!list->string) {
        list->string = string;
        return;
    }
    if (!list->right) {
        list->right = newList(list);
    }
    push_back(list->right, string);
    return;
}
