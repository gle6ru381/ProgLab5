#include "input.h"
#include <ctype.h>
#include <stdlib.h>

List* input()
{
    FILE* file;
    printf("input file name: ");
    char name[261] = "bdc.txt";
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

void printError(char const* error, char const* path, char const* name)
{
    fprintf(stderr, "%s", error);
    if (name)
        fprintf(stderr, " %s in path %s", name, path);
    else
        fprintf(stderr, " %s", path);
}

#define shortError(error, path) printError(error, path, NULL)

void check(List* pathList)
{
    char pathSep;
    for (List* i = pathList; i; i = i->right) {
        bool absolute = false;
        if (strLen(i->string) > 260) {
            shortError("too bog file path", i->string);
            exit(EXIT_FAILURE);
        }
        if (strContains(i->string, '/')) {
            if (strContains(i->string, '\\')) {
                shortError("incorrect path", i->string);
                exit(EXIT_FAILURE);
            }
            pathSep = '/';
        }
        if (strContains(i->string, '\\')) {
            if (strContains(i->string, '/') || i->string[0] == '\\') {
                shortError("incorrect path", i->string);
                exit(EXIT_FAILURE);
            }
            pathSep = '\\';
        }
        if (i->string[0] != '/') {
            if (isupper(i->string[0])) {
                if (i->string[1] == ':' && i->string[2] != '\\') {
                    shortError("incorrect windows path", i->string);
                    exit(EXIT_FAILURE);
                } else {
                    i->string += 2;
                    i->win = true;
                }
            }
        }
        if (strContains(i->string, ':') || strContains(i->string, '|')
            || strContains(i->string, '"') || strContains(i->string, '*')
            || strContains(i->string, '?') || strContains(i->string, '<')
            || strContains(i->string, '>')) {
            shortError("incorrect path", i->string);
            exit(EXIT_FAILURE);
        }
        if (strStr(i->string, ".../")->string || strStr(i->string, " /")->string
            || strStr(i->string, "...\\")->string
            || strStr(i->string, " \\")->string) {
            shortError("incorrecr path", i->string);
            exit(EXIT_FAILURE);
        }

        if (i->string[0] == pathSep) {
            absolute = true;
            i->string++;
        }

        List* nameList = strSplit(i->string, pathSep);
        for (List* name = nameList; name; name = name->right) {
            if (strLen(name->string) == 0) {
                if (name->right || !strCmp(name->left->string, "..")) {
                    printError("incorrect dir", i->string, name->string);
                    exit(EXIT_FAILURE);
                } else
                    continue;
            }
            if (isalpha(name->string[0])
                && name->string[strLen(name->string) - 1] == '.'
                && !strCmp(name->string, "..")) {
                printError("name end with dot", i->string, name->string);
                exit(EXIT_FAILURE);
            }
        }
        if (i->win) {
            i->string -= 2;
        }
        if (absolute) {
            i->string--;
        }
    }
}

char* removeDotDot(char* string, char pathSep, bool win)
{
    char* substr = pathSep == '/' ? "/../" : "\\..\\";
    char const* temp = strCpy(string);
    if (win) {
        string += 2;
    }

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
                if (i == 0) {
                    shortError("root directory exit", temp);
                    exit(EXIT_FAILURE);
                }
                i -= 1;
                while (string[i] != pathSep && i != 0) {
                    i--;
                }
                strRemove(string, string + i, string + j - 2);
                i--;
            }
        }
    }

    if (win) {
        string -= 2;
    }
    return string;
}

void process(List* pathList)
{
    for (List* i = pathList; i; i = i->right) {
        char pathSep;
        if (strContains(i->string, '/')) {
            pathSep = '/';
            i->string = removeDotDot(i->string, '/', false);
            i->string = strReplace(i->string, "./");
        } else {
            pathSep = '\\';
            i->string = removeDotDot(i->string, '\\', i->win);
            i->string = strReplace(i->string, ".\\");
        }
        uint length = strLen(i->string);
        if (i->string[length - 1] == '.') {
            if (i->win) {
                i->string += 2;
                length -= 2;
            }
            uint end = length - 1;
            uint begin = end - 3;
            if (begin <= 0) {
                shortError("incorrect path", "?");
                exit(EXIT_FAILURE);
            }
            while (begin != 1) {
                if (i->string[begin] == pathSep) {
                    break;
                }
                begin--;
            }
            strRemove(i->string, i->string + begin, i->string + end);
            if (i->win) {
                i->string -= 2;
            }
        }
    }
}
