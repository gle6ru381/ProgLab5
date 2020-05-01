#ifndef LIST_H
#define LIST_H

typedef unsigned uint;

struct List {
    char* string;
    struct List* right;
    struct List* left;
};

typedef struct List List;

List* newList(List* parent);
void push_back(struct List* list, char* value);

#define initList() newList(NULL)

#endif // LIST_H
