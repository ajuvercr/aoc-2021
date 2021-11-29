#pragma once

typedef struct Cursor Cursor;
typedef struct Rule Rule;
typedef void *(create)(const Cursor *cur, Rule rule);

typedef enum { And, Or, Lit, Sym } EbnfType;

struct Rule {
    EbnfType type; 
    const char *lit;
    const char *symbol;
    Rule *alpha;
    Rule *beta;
};

struct Cursor {
    const char *buf;
    int at;
};

Rule *CreateParser(Cursor *cursor);
void printRule(Rule *rule, int indent);
void FreeRule(Rule *rule);


void trimWhite(Cursor *cursor);
const char *parseWith(Cursor *cursor, int f(Cursor *c));

