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

    if (strchr("+-*/()", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur      = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "invalid token");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

typedef enum {
  NODE_ADD,  // +
  NODE_SUB,  // -
  NODE_MUL,  // *
  NODE_DIV,  // /
  NODE_NUM,  // Number
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;  // Node kind
  Node *lhs;      // Left-hand side
  Node *rhs;      // Right-hand side
  int val;        // if kind equals NODE_NUM, it has a value
};

/**
 * Create a new node.
 *
 * @param kind Node kind
 *
 * @return New node
 */
Node *new_node(NodeKind kind) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  return node;
}

/**
 * Create a new binary.
 *
 * @param kind Node kind
 * @param lhs Left-hand side
 * @param rhs Right-hand side
 *
 * @return New node
 */
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  node->lhs  = lhs;
  node->rhs  = rhs;
  return node;
}

/**
 * Create a new number.
 *
 * @param val Value
 *
 * @return New node
 */
Node *new_num(int val) {
  Node *node = new_node(NODE_NUM);
  node->val  = val;
  return node;
}

Node *expr();
Node *mul();
Node *primary();

/**
 * Expression of basic arithmetic operations.
 *
 * expr = mul ("+" mul | "-" mul)*
 * mul = primary ("*" primary | "/" primary)*
 * primary = num | "(" expr ")"
 *
 * @return Parsed node
 */
Node *expr() {
  Node *node = mul();

  while (true) {
    if (consume('+'))
      node = new_binary(NODE_ADD, node, mul());
    else if (consume('-'))
      node = new_binary(NODE_SUB, node, mul());
    else
      return node;
  }
}

/**
 * Expression of multiplication and division.
 *
 * @return Parsed node
 */
Node *mul() {
  Node *node = primary();

  while (true) {
    if (consume('*'))
      node = new_binary(NODE_MUL, node, primary());
    else if (consume('/'))
      node = new_binary(NODE_DIV, node, primary());
    else
      return node;
  }
}

/**
 * Primary expression.
 *
 * @return Parsed node
 */
Node *primary() {
  // case: (expr)
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }

  // case: number
  return new_num(expect_number());
}

/**
 * Generate assembly code.
 *
 * @param node Parsed node
 */
void gen(Node *node) {
  if (node->kind == NODE_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
    case NODE_ADD:
      printf("  add rax, rdi\n");
      break;
    case NODE_SUB:
      printf("  sub rax, rdi\n");
      break;
    case NODE_MUL:
      printf("  imul rdi\n");
      break;
    case NODE_DIV:
      printf("  cqo\n");  // Convert RAX(64bit) to RDX:RAX(128bit)
      printf("  idiv rdi\n");
      break;
  }

  printf("  push rax\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("%s: Not correct number of arguments", argv[0]);
  }

  user_input = argv[1];
  token      = tokenize(user_input);
  Node *node = expr();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // Generate code using the recursive descent parser
  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
