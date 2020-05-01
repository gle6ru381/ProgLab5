#include "input.h"
#include "stringoperation.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    List* pathList = input();
    check(pathList);
    process(pathList);
    for (List* i = pathList; i; i = i->right) {
        printf("%s\n", i->string);
    }
    system("pause");
    return 0;
}
