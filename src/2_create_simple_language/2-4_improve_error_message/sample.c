#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TK_RESERVED,  // Symbol
  TK_NUM,       // Integer token
  TK_EOF,       // End of input token
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;  // Token kind
  Token *next;     // Next token
  int val;         // If kind is TK_NUM, its value
  char *str;       // Token string
};

// Input program
char *user_input;

// Current token
Token *token;

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
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) return false;
  token = token->next;
  return true;
}

/**
 * Ensure that the current token is op.
 *
 * @param op Operator
 */
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "expected '%c'", op);
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
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind  = kind;
  tok->str   = str;
  cur->next  = tok;
  return tok;
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

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur      = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "expected a number");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("Not correct number of arguments");
    return 1;
  }

  user_input = argv[1];
  token      = tokenize();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
  printf("  mov rax, %d\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
