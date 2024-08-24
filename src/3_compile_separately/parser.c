#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "c_compiler.h"

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
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

/**
 * Expression of basic arithmetic operations.
 *
 * expr = equality
 * equality = relational ("==" relational | "!=" relational)*
 * relational = add ("<" add | "<=" add | ">" add | ">=" add)*
 * add = mul ("+" mul | "-" mul)*
 * mul = unary ("*" unary | "/" unary)*
 * unary = ("+" | "-")? primary
 * primary = num | "(" expr ")"
 *
 * @return Parsed node
 */
Node *expr() {
  return equality();
}

/**
 * Equality expression.
 *
 * @return Parsed node
 */
Node *equality() {
  Node *node = relational();

  while (true) {
    if (consume("=="))
      node = new_binary(NODE_EQ, node, relational());
    else if (consume("!="))
      node = new_binary(NODE_NE, node, relational());
    else
      return node;
  }
}

/**
 * Relational expression.
 *
 * @return Parsed node
 */
Node *relational() {
  Node *node = add();

  while (true) {
    if (consume("<"))
      node = new_binary(NODE_LT, node, add());
    else if (consume("<="))
      node = new_binary(NODE_LE, node, add());
    else if (consume(">"))
      node = new_binary(NODE_LT, add(), node);
    else if (consume(">="))
      node = new_binary(NODE_LE, add(), node);
    else
      return node;
  }
}

/**
 * Expression of addition and subtraction.
 *
 * @return Parsed node
 */
Node *add() {
  Node *node = mul();

  while (true) {
    if (consume("+"))
      node = new_binary(NODE_ADD, node, mul());
    else if (consume("-"))
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
  Node *node = unary();

  while (true) {
    if (consume("*"))
      node = new_binary(NODE_MUL, node, unary());
    else if (consume("/"))
      node = new_binary(NODE_DIV, node, unary());
    else
      return node;
  }
}

/**
 * Unary operator expression.
 *
 * @return Parsed node
 */
Node *unary() {
  if (consume("+")) return primary();
  if (consume("-"))
    return new_binary(NODE_SUB, new_num(0), primary());  // -x = 0 - x
  return primary();
}

/**
 * Primary expression.
 *
 * @return Parsed node
 */
Node *primary() {
  // case: (expr)
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  // case: number
  return new_num(expect_number());
}
