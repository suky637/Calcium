#include <string.h>
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
