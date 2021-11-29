#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ebnf_parser.h"

void printRule(Rule *rule, int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
  switch (rule->type) {
  case And:
    printf("AND\n");
    printRule(rule->alpha, indent + 1);
    printRule(rule->beta, indent + 1);
    break;

  case Or:
    printf("OR\n");
    printRule(rule->alpha, indent + 1);
    printRule(rule->beta, indent + 1);
    break;
  case Lit:
    printf("Lit %s\n", rule->lit);
    break;
  case Sym:
    printf("Sym %s\n", rule->symbol);
    break;
  }
}

char cursorGet(Cursor *cursor) { return cursor->buf[cursor->at]; }
char cursorPref(Cursor *cursor) {
  if (cursor->at > 0)
    return cursor->buf[cursor->at - 1];
  return '\0';
}

int isReserved(Cursor *cursor) {
  char current = cursorGet(cursor);
  return current == '|' || current == '(' || current == ')' || current == '\0';
}

int contWhite(Cursor *cursor) {
  char current = cursorGet(cursor);
  return current == ' ' || current == '\n' || current == '\t';
}

int contLiteral(Cursor *cursor) {
  char current = cursorGet(cursor);
  return current != '"' || cursorPref(cursor) == '\\';
}

int contSymbol(Cursor *cursor) {
  return !contWhite(cursor) && !isReserved(cursor);
}

void trimWith(Cursor *cursor, int f(Cursor *c)) {
  while (f(cursor))
    cursor->at++;
}

void trimWhite(Cursor *cursor) { trimWith(cursor, contWhite); }

const char *parseWith(Cursor *cursor, int f(Cursor *c)) {
  const int at = cursor->at;
  trimWith(cursor, f);
  const int to = cursor->at;

  printf("%d->%d\n", at, to);
  char *out = calloc(0, to - at + 1); // Add null terminating char
  memcpy(out, &cursor->buf[at], to - at);
  printf("parsed %s\n", out);
  return out;
}

Rule *CreateParser(Cursor *cursor) {
  Rule *rule = malloc(sizeof(Rule));

  trimWhite(cursor);

  if (!isReserved(cursor)) {
    if (cursorGet(cursor) == '"') {
      cursor->at++;
      rule->lit = parseWith(cursor, contLiteral);
      cursor->at++;
      rule->type = Lit;
    } else {
      rule->symbol = parseWith(cursor, contSymbol);
      rule->type = Sym;
    }
  }

  trimWhite(cursor);

  char current = cursorGet(cursor);
  if (current == '\0')
    return rule;

  Rule *parent = malloc(sizeof(Rule));

  switch (current) {
  case '|':
    parent->type = Or;
    cursor->at ++;
    break;
  default:
    parent->type = And;
    break;
  }

  parent->alpha = rule;
  trimWhite(cursor);
  parent->beta = CreateParser(cursor);

  rule = parent;

  return rule;
}

void FreeRule(Rule *rule) {
  if (rule->lit)
    free((char *)rule->lit);
  if (rule->symbol)
    free((char *)rule->symbol);
  if (rule->alpha)
    FreeRule(rule->alpha);
  if (rule->beta)
    FreeRule(rule->beta);
  free(rule);
}
