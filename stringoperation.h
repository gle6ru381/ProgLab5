#ifndef STRINGOPERATION_H
#define STRINGOPERATION_H

#include "list.h"

uint strLen(char const* const str);
List* strSplit(char const* const str, char const separator);
bool strContains(char const* const str, char const sym);
List* strStr(char const* const string, char const* const substr);
bool strCmp(char const* const str1, char const* const str2);
char* strCpy(char const* const copyStr);
void strRemove(char* string, char* begin, char* end);
char* strReplace(char* string, char* substr);

#endif // STRINGOPERATION_H
