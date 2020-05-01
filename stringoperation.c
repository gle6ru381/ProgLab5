#include "stringoperation.h"
#include "list.h"
#include <malloc.h>
#include <stdlib.h>

uint strLen(char const* str)
{
    for (uint length = 0;; length++) {
        if (!str[length])
            return length;
    }
}

char* strCpy(char const* const strCopy)
{
    char* newStr = calloc(strLen(strCopy) + 1, sizeof(char));

    for (uint i = 0; strCopy[i]; i++) {
        newStr[i] = strCopy[i];
    }
    return newStr;
}

List* strSplit(char const* const string, char const separator)
{
    char* tempStr = strCpy(string);
    List* list = initList();

    uint previ = 0;
    for (uint i = 0; string[i]; i++) {
        if (string[i] == separator) {
            tempStr[i] = 0;
            push_back(list, strCpy(tempStr + previ));
            previ = i + 1;
        }
    }
    push_back(list, strCpy(tempStr + previ));
    return list;
}

bool strContains(char const* const string, char const sym)
{
    for (uint i = 0; string[i]; i++) {
        if (string[i] == sym) {
            return true;
        }
    }
    return false;
}

List* strStr(char const* const string, char const* const substr)
{
    List* list = initList();
    char* tempStr = strCpy(string);

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
                push_back(list, tempStr + i);
            }
            i = j;
        }
    }
    return list;
}

bool strCmp(char const* const str1, char const* const str2)
{
    for (uint i = 0; str1[i] || str2[i]; i++) {
        if (str1[i] != str2[i])
            return false;
    }
    return true;
}

void strRemove(char* string, char* begin, char* end)
{
    if (!*(end + 1)) {
        *begin = 0;
        return;
    }
    uint tail = (uint)(strLen(string) - (end - string));

    for (uint i = 0; i < tail; i++) {
        begin[i] = end[i + 1];
    }
}

char* strReplace(char* string, char* substr)
{
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
                strRemove(string, string + i, string + (j - 1));
            }
        }
    }
    return string;
}
