#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

/************************
 * Token
 ************************/

// Token kind
typedef enum {
  TK_RESERVED,  // Symbol
  TK_NUM,       // Integer token
  TK_EOF,       // End of input token
} TokenKind;

// Token
typedef struct Token Token;
struct Token {
  TokenKind kind;  // Token kind
  Token *next;     // Next token
  int val;         // If kind is TK_NUM, its value
  char *str;       // Token string
  int len;         // Token length
};

// Input program
extern char *user_input;

// Current token
extern Token *token;

noreturn void error(char *fmt, ...);
noreturn void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number(void);
bool at_eof(void);
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool starts_with(char *p, char *q);
Token *tokenize(void);

/************************
 * Node
 ************************/

// Node kind
typedef enum {
  NODE_ADD,  // +
  NODE_SUB,  // -
  NODE_MUL,  // *
  NODE_DIV,  // /
  NODE_EQ,   // ==
  NODE_NE,   // !=
  NODE_LT,   // <
  NODE_LE,   // <=
  NODE_NUM,  // Number
} NodeKind;

// Node
typedef struct Node Node;
struct Node {
  NodeKind kind;  // Node kind
  Node *lhs;      // Left-hand side
  Node *rhs;      // Right-hand side
  int val;        // if kind equals NODE_NUM, it has a value
};

Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_num(int val);
Node *expr(void);

/************************
 * Generate code
 ************************/

void pop(char *arg);
void push(void);
void ret(void);
void gen_header(void);
void gen(Node *node);
