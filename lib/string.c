#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct string{
char *val;
};

 struct string  string__create(char *value){
struct string x;
x.val = value;
return x;
};

 struct string  string__set(struct string val, char *value){
val.val = value;
};

 int  string__length(struct string val){
int ret;
ret = strlen(val.val);
return ret;
};

 struct string  string__removeAt(struct string val, int index){
int size = string__length(val);
if (size<index)return val;
char *ret = malloc(sizeof(char)*(size-1));
int i = 0;
int j = 0;
while (i<size
){
if (i!=index
){
ret[j] = val.val[i];
j = j+1;
};
i = i+1;
};
struct string buf = string__create(ret);
return buf;
};

 struct string  string__pushChar(struct string val, char c){
int size = string__length(val);
char *ret = malloc(sizeof(char)*(size+1));
int i = 0;
while (i<size
){
ret[i] = val.val[i];
i = i+1;
};
ret[i] = c;
struct string buf = string__create(ret);
return buf;
};
