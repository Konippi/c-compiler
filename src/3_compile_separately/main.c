#include <stdio.h>
#include <stdlib.h>

#include "c_compiler.h"

char *user_input;
Token *token;

int main(int argc, char **argv) {
  if (argc != 2) {
    error("%s: Not correct number of arguments", argv[0]);
  }

  user_input = argv[1];
  token      = tokenize();
  Node *node = expr();

  // Generate code
  gen_header();
  gen(node);

  pop("rax");
  ret();
  return 0;
}
