#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct string{
char *val;
};
typedef struct string string;

 struct string  string__create(char *value);
 struct string  string__set(struct string val, char *value);
 int  string__length(struct string val);
 struct string  string__removeAt(struct string val, int index);
 struct string  string__pushChar(struct string val, char c);
 struct string  string__pushString(struct string val, struct string add);
 struct string  string__removeFrom(struct string val, int startInd, int endInd);