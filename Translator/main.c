
#include "global.h"

//Symbol section
#define STRMAX 999
#define SYMMAX 100

char lexemes[STRMAX];
int lastchar = - 1;
struct entrie symtable[SYMMAX];
int lastentry = 0;

int lookup (char s[])
{
    int p;
    for (p = lastentry; p>0; p--)
    {
        if (strcmp(symtable[p].lexptr, s) == 0)
        	{
        		return p;
			}
	}
    return 0;
}

int insert (char s[], int tok)
{
    int len;
    len = strlen(s);
    if (lastentry + 1 >= SYMMAX)
        error("symbol table full");
    if (lastchar + len + 1 >= STRMAX)
        error("lexemes array full");
    lastentry++;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar+ 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr,s);
    return lastentry;
}


//Parser section

void expr(void);
void term(void);
void factor(void);
void prop(void);

int lookahead;

void parse(void)
{
    lookahead = lexan();
    while (lookahead != DONE) {
        prop();
        expr();
        match(';');
    }
}

void expr(void)
{
    int t;
    term();
    while (1) {
        switch (lookahead) {
            case '+': case '-': 
            ///extras:
            case'=':  case'<': case'>': 
                t = lookahead;
                match(lookahead);
                term();
                emit(t,NONE);
                continue;
            default:
                return;
        }
    }
}

void term(void)
{
    int t;
    factor();
    while (1) {
        switch (lookahead) {
            case '*': case '/': case DIV: case MOD:
            ///extras
             case AND: case OR:
                t=lookahead;
                match(lookahead);
                factor();
                emit(t,NONE);
                continue;
            default:
                return;
        }
    }
}

void factor(void)
{
    switch (lookahead) {
        case '(':
            match ('(');
            expr();
            match(')');
            break;
        case ('{'):
            match ('{');
            expr();
            match('}');
            break;
        ///extras
        case INT:
            match(INT);
            expr();
            emit(INT, tokenval);
            prop();
        break;

        case NUM:
            emit(NUM, tokenval);
            match(NUM);
            break;
        case ID:
            emit(ID, tokenval);
            match(ID);
            break;
        default:
            error ("Syntax error");
    }
}

void prop(void ){
  switch (lookahead){
    case IF:
      match(IF);
      expr();
      emit(IF,tokenval); // Print IF
      match(THEN);
      emit(THEN,tokenval); // Print THEN
      prop();

    break;
    case WHILE:
      match(WHILE);
      expr();
      emit(WHILE,tokenval); // Print WHILE
      match(DO);
      emit(DO,tokenval);  // Print DO
      prop();
    break;
    case BEGIN:
        match(BEGIN);
        prop();
        emit(BEGIN, tokenval); // Print BEGIN
    break;
    case END:
        emit(END, tokenval); // Print END
        exit(0);
    break;
   

  default:
    asm("nop");
    break;
  }
}

void match (int t)
{
    if (lookahead==t)
    {
    	lookahead=lexan();
	}
    else error ("Syntax error");
}

//Lexer section

char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;

int lexan(void){
    int t;
    while (1) {
        t = getchar();
        if (t == ' ' || t == '\t')
            ;
        else if (t == '\n')
            lineno++;
        else if (isdigit(t)) {
            ungetc(t, stdin);
            scanf("%d", &tokenval);
            return NUM;
        }
        else if (isalpha(t)) {
            int p, b=0;
            while (isalnum(t)) {
            	
                lexbuf[b] = t;
                t = getchar();
                b++;
                if (b>=BSIZE)
                    error ("compiler error");
            }
            lexbuf[b] = EOS;
            if (t != EOF)
                ungetc(t, stdin);
            p = lookup(lexbuf);
            if (p == 0)
                p = insert(lexbuf, ID);
            tokenval = p;
            return symtable[p].token;
        }
        else if (t == EOF)
            return DONE;
        else {
            tokenval = NONE;
            return t;
        }
    }
}

//Emitter section

void emit(int t, int tval)
{
    switch (t) {
        case '+': case '-': case '*': case '/': 
        ///extra cases
        case '=': case'<': case'>': 
            printf("\"%c\" = operand symbol\n", t);
            break;
        case IF:
            printf ("IF = conditional\n" );
            break;
        case THEN:
            printf ("THEN = conditional\n" );
            break;
        case WHILE:
            printf ("WHILE = loop\n" );
            break;
        case DO:
            printf ("DO = conditional\n" );
            break;
        case DIV:
            printf ("DIV = division\n" );
            break;
        case MOD:
            printf ("MOD = module\n" );
            break;
        case AND: ///extra
            printf ("AND = logical and\n" );
            break;
        case OR: ///extra
            printf ("OR = logical or\n" );
            break;
        case INT: ///extra
            printf ("INT = variable\n" );
            break;                  
        case BEGIN:
            printf("[+] BEGIN [+]\n");
            break;
        case END:
            printf("[-] END [-]\n");
            break;               
        case NUM:
            printf ("number = %d\n" , tval);
            break;
        case ID:
            printf("string = %s\n", symtable[tval].lexptr);
            break;
        default:
            printf ( "token %d, tokenval %d\n", t, tval);
    }
}

//Error section

void error (char * m)
{
    fprintf(stderr, "line %d: %s\n" , lineno, m);
    exit(1);
}

//Init section

struct entrie keywords[] = {
	//"if", IF,
    "div", DIV,
    "mod", MOD,
    "if", IF,
    "then", THEN,
    "while", WHILE,
    "do", DO,
    "end", END,
    "begin", BEGIN,
    "int", INT,
    "and", AND,
    "or", OR,
    0,0
};

void init(void)
{
    struct entrie *p;
    for (p = keywords; p->token; p++)
        insert(p->lexptr, p->token);
}

int main(int argc, const char * argv[])
{
    init();
    parse();
    exit(0);
}

