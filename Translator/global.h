
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define BSIZE 512
#define NONE -1
#define EOS '\0'
#define NUM 252
#define DIV 253
#define MOD 254
#define AND 255 ///exta
#define OR 256 ///extra
#define ID 257

#define IF 258 
#define THEN 259 
#define WHILE 260 
#define DO 261 
#define BEGIN 263 
#define END 264 
#define INT 265 /// extra


#define DONE 268


struct entrie {
    char * lexptr;
    int token;
};

extern int tokenval;
extern int lineno;
extern struct entrie symtable[];

int insert (char s[], int tok);
int lexan(void);
int lookup (char s[]);
void match (int t);
void emit(int t, int tval);
void error (char * m);
