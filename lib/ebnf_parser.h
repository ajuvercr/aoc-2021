#pragma once
#include "map.h"

typedef struct Cursor Cursor;
typedef struct Rule Rule;
typedef struct Parset Parset;
typedef void *(create)(const Cursor *cur, Rule rule);

typedef enum { And, Or, Lit, Sym } EbnfType;
typedef enum { Terminal, NonTerminal } ParsetType;

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

struct Parset {
    ParsetType type;
    int matched[2]; // Maybe allocated and never freed
    const char *symbol;

    Parset *child;
    Parset *beta;
};

Rule *CreateParser(Cursor *cursor);
void printRule(Rule *rule, int indent);
void FreeRule(Rule *rule);


void trimWhite(Cursor *cursor);
const char *parseWith(Cursor *cursor, int f(Cursor *c));

void printParset(Parset *, const char*);
Parset *parse(Map *rules, Rule *rule, Cursor *);

