#include "input.h"
#include <ctype.h>
#include <stdlib.h>

List* input()
{
    FILE* file;
    printf("input file name: ");
    char name[261];
    scanf("%s", name);
    getchar();
    file = fopen(name, "r");
    if (!file) {
        fprintf(stderr, "file not open\n");
        exit(EXIT_FAILURE);
    }
    printf("input paths separator: ");
    char separator = getchar();
    getchar();

    char* paths = calloc(10000, sizeof(char));
    fgets(paths, 10000, file);

    return strSplit(paths, separator);
}

void check(List* pathList)
{
    bool win = false;
    char pathSep;
    for (List* i = pathList; i; i = i->right) {
        if (strLen(i->string) > 260) {
            fprintf(stderr, "too big file path");
            exit(EXIT_FAILURE);
        }
        if (strContains(i->string, '/')) {
            if (strContains(i->string, '\\')) {
                fprintf(stderr, "incorrect path");
                exit(EXIT_FAILURE);
            }
            pathSep = '/';
        }
        if (strContains(i->string, '\\')) {
            if (strContains(i->string, '/')) {
                fprintf(stderr, "incorrect path");
                exit(EXIT_FAILURE);
            }
            pathSep = '\\';
        }
        if (i->string[0] != '/') {
            if (isupper(i->string[0])) {
                if (i->string[1] == ':' && i->string[2] != '\\') {
                    fprintf(stderr, "incorrect windows path");
                    exit(EXIT_FAILURE);
                } else {
                    i->string += 2;
                    win = true;
                }
            }
        }
        if (strContains(i->string, ':') || strContains(i->string, '|')
            || strContains(i->string, '"') || strContains(i->string, '*')
            || strContains(i->string, '?') || strContains(i->string, '<')
            || strContains(i->string, '>')) {
            fprintf(stderr, "incorrect path");
            exit(EXIT_FAILURE);
        }
        if (strStr(i->string, ".../")->string || strStr(i->string, " /")->string
            || strStr(i->string, "...\\")->string
            || strStr(i->string, " \\")->string) {
            fprintf(stderr, "incorrecr path");
            exit(EXIT_FAILURE);
        }
        List* nameList = strSplit(i->string, pathSep);
        for (List* name = nameList; name; name = name->right) {
            if (strLen(name->string) == 0) {
                fprintf(stderr, "incorrect dir");
                exit(EXIT_FAILURE);
            }
            if (isalpha(name->string[0])
                && name->string[strLen(name->string) - 1] == '.') {
                fprintf(stderr, "name end with dot");
                exit(EXIT_FAILURE);
            }
        }
        if (win) {
            i->string -= 2;
        }
    }
}

char* removeDotDot(char* string, char pathSep)
{
    char* substr = pathSep == '/' ? "/../" : "\\..\\";

    for (uint i = 0; string[i]; i++) {
        if (string[i] == *substr) {
            bool flag = true;
            uint j = i + 1;
            for (uint subi = 1; substr[subi]; j++, subi++) {
                if (string[j] != substr[subi]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                i -= 2;
                while (string[i] != pathSep && i != 0) {
                    i--;
                }
                strRemove(string, string + i, string + j - 2);
            }
        }
    }
    return string;
}

void process(List* pathList)
{
    for (List* i = pathList; i; i = i->right) {
        i->string = removeDotDot(i->string, '/');
        i->string = removeDotDot(i->string, '\\');
        i->string = strReplace(i->string, "./");
        i->string = strReplace(i->string, ".\\");
    }
}
