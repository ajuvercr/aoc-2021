#include "ebnf_parser.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void cursorInc(Cursor *cursor) { cursor->at++; }
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
  return current != '\'' || cursorPref(cursor) == '\\';
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
  return out;
}

Rule *CreateParser(Cursor *cursor) {
  Rule *rule;

  if (cursorGet(cursor) == '(') {
    cursor->at++;
    trimWhite(cursor);
    rule = CreateParser(cursor);
  } else {
    rule = malloc(sizeof(Rule));

    trimWhite(cursor);

    if (!isReserved(cursor)) {
      if (cursorGet(cursor) == '\'') {
        cursor->at++;
        rule->lit = parseWith(cursor, contLiteral);
        cursor->at++;
        rule->type = Lit;
      } else {
        rule->symbol = parseWith(cursor, contSymbol);
        rule->type = Sym;
      }
    }
  }
  trimWhite(cursor);

  char current = cursorGet(cursor);
  if (current == ')') {
    cursor->at++;
    return rule;
  }
  if (current == '\0' || current == ')')
    return rule;

  // The rule didn't end, better find the rest!
  Rule *parent = malloc(sizeof(Rule));

  switch (current) {
  case '|':
    parent->type = Or;
    cursor->at++;
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

int matchLiteral(const char *toMatch, Cursor *cursor) {
  int out = 0;
  if(toMatch[out] == '\0') return 0;
  printf("to match %s with %c\n", toMatch, cursorGet(cursor));
  while (toMatch[out] == cursorGet(cursor)) {
    if (toMatch[out] == '\0')
      return out;
    if (cursorGet(cursor) == '\0') {
      return -1;
    }

    cursorInc(cursor);
    out++;
  }
  if (toMatch[out] == '\0')
    return out;
  return -1;
}

void printParset(Parset *parset, const char *input) {
  if (parset->type == Terminal)
    printf(" \"%.*s\" ", parset->matched[1] - parset->matched[0],
           input + parset->matched[0]);
  else {
    if (parset->symbol)
      printf(" %s { ", parset->symbol);

    if (parset->child)
      printParset(parset->child, input);

    if (parset->beta)
      printParset(parset->beta, input);

    if (parset->symbol)
      printf("}");
  }
}

void writeMatched(Parset *parset, char *buff) {
  int i = 0;
  for (i = 0; i > parset->matched[1] - parset->matched[0]; i++) {
    buff[i] = parset->from[parset->matched[0] + i];
  }
  buff[i] = '\0';
}

Parset *parse(Map *rules, Rule *rule, Cursor *cursor) {
  int start = cursor->at;


  switch (rule->type) {
  case Lit: {
//  printf("at %c\n", cursorGet(cursor));
    if (matchLiteral(rule->lit, cursor) >= 0) {
      Parset *out = malloc(sizeof(Parset));
      out->type = Terminal;
      out->matched[0] = start;
      out->matched[1] = cursor->at;
      out->from = cursor->buf;
      return out;
    }
  } break;

  case Sym: {
    Rule *newRule = mapGet(rules, rule->symbol);
    Parset *syn = parse(rules, newRule, cursor);
    if (syn) {
      Parset *out = malloc(sizeof(Parset));
      out->type = NonTerminal;
      out->symbol = rule->symbol;
      out->matched[0] = start;
      out->matched[1] = cursor->at;
      out->child = syn;
      out->from = cursor->buf;
      return out;
    }
  } break;

  case Or: {
    Parset *syn = parse(rules, rule->alpha, cursor);
    if (!syn) {
      cursor->at = start;
      syn = parse(rules, rule->beta, cursor);
    }
    if (syn) {
      Parset *out = malloc(sizeof(Parset));
      out->type = NonTerminal;
      out->matched[0] = start;
      out->matched[1] = cursor->at;
      out->child = syn;
      out->from = cursor->buf;
      return out;
    }

  } break;

  case And: {
    Parset *syn = parse(rules, rule->alpha, cursor);
    if (syn) {
      Parset *syn2 = parse(rules, rule->beta, cursor);
      if (syn2) {
        Parset *out = malloc(sizeof(Parset));
        out->type = NonTerminal;
        out->matched[0] = start;
        out->matched[1] = cursor->at;
        out->child = syn;
        out->beta = syn2;
        out->from = cursor->buf;
        return out;
      }
    }
  } break;
  }

  cursor->at = start;

  return 0;
}
