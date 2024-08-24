#include <stdio.h>
#include <stdlib.h>

#include "c_compiler.h"

/**
 * Report an error and exit.
 *
 * @param fmt Error message format
 * @param ... Error message format arguments
 */
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

/**
 * Report an error and exit.
 *
 * @param loc Error location
 * @param fmt Error message format
 * @param ... Error message format arguments
 */
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " ");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

/**
 * Check if the current token is op.
 *
 * @param op Operator
 *
 * @return Is the current token op
 */
bool consume(char *op) {
  if (token->kind != TK_RESERVED || strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

/**
 * Ensure that the current token is op.
 *
 * @param op Operator
 */
void expect(char *op) {
  if (token->kind != TK_RESERVED || strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    error_at(token->str, "expected \"%s\"", op);
  token = token->next;
}

/**
 * Get a number and move to the next token.
 *
 * @return Number
 */
int expect_number() {
  if (token->kind != TK_NUM) error_at(token->str, "expected a number");
  int val = token->val;
  token   = token->next;
  return val;
}

/**
 * Check if the current token is EOF.
 *
 * @return Is the current token EOF
 */
bool at_eof() {
  return token->kind == TK_EOF;
}

/**
 * Create a new token.
 *
 * @param kind Token kind
 * @param cur Current token
 * @param str Token string
 *
 * @return New token
 */
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind  = kind;
  tok->str   = str;
  tok->len   = len;
  cur->next  = tok;
  return tok;
}

/**
 * Check if the input string starts with a given string.
 *
 * @param p Input string
 * @param q Start string
 *
 * @return Does the input string start with the given string
 */
bool starts_with(char *p, char *q) {
  return memcmp(p, q, strlen(q)) == 0;
}

/**
 * Tokenize input string.
 *
 * @param p Input string
 *
 * @return Tokenized tokens
 */
Token *tokenize() {
  char *p = user_input;
  Token head;
  head.next  = NULL;
  Token *cur = &head;

  while (*p) {
    // Skip whitespace characters
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Multi-letter punctuator
    if (starts_with(p, "==") || starts_with(p, "!=") || starts_with(p, "<=") ||
        starts_with(p, ">=")) {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p += 2;
      continue;
    }

    // Single-letter punctuator
    if (strchr("+-*/()<>", *p)) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      cur      = new_token(TK_NUM, cur, p, 0);
      char *q  = p;
      cur->val = strtol(p, &p, 10);
      cur->len = p - q;
      continue;
    }

    error_at(p, "invalid token");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}
