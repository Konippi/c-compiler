#include <stdio.h>
#include <stdlib.h>

#include "c_compiler.h"

/**
 * Pop the stack.
 *
 * @param arg Register
 */
void pop(char *arg) {
  printf("  pop %s\n", arg);
}

/**
 * Push the stack.
 */
void push(void) {
  printf("  push rax\n");
}

/**
 * Return from the function.
 */
void ret(void) {
  printf("  ret\n");
}

/**
 * Generate assembly code header.
 */
void gen_header(void) {
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
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

  pop("rdi");
  pop("rax");

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
      printf("  cqo\n");       // Convert RAX(64bit) to RDX:RAX(128bit)
      printf("  idiv rdi\n");  // RAX=RAX/RDI, RDX=RAX%RDI
      break;
    case NODE_EQ:
      printf("  cmp rax, rdi\n");  // Comparison result is set to FLAGS register
      printf("  sete al\n");       // Set AL(lower 8 bits of RAX) to 1 if equal
      printf("  movzb rax, al\n");  // Zero clear upper 56 bits of RAX
      break;
    case NODE_NE:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case NODE_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case NODE_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }

  push();
}
