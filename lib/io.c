#include <stdio.h>
#include <stdlib.h>

 char *  IO__read(char *filename){
FILE *file = fopen(filename,"r");
if (!file
){
printf("IO.read(point char): Couldn't open file.\n");
return NULL;
};
fseek(file,0,SEEK_END);
int length = ftell(file);
fseek(file,0,SEEK_SET);
char *content = malloc(length+1);
if (!content
){
printf("IO.read(point char): Couldn't allocate memory.\n");
fclose(file);
return NULL;
};
fread(content,1,length,file);
content[length] = '\0';
fclose(file);
return content;
};

 int  IO__write(char *filename, char *text){
FILE *file = fopen(filename,"w");
if (!file
){
printf("IO.write(point char, point char): Couldn't open file for writing.\n");
return 1;
};
int result = fputs(text,file);
if (result==EOF
){
printf("IO.write(point char, point char): Couldn't write to file.\n");
return 1;
};
fclose(file);
return 0;
};
