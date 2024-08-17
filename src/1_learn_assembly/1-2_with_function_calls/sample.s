.intel_syntax noprefix
.globl plus, main

plus:
    add rsi, rdi # first argument is stored in RDI, second argument is stored in RSI
    mov rax, rsi # return value is stored in RAX
    ret          # return from plus function (pop the address from the stack, jump to the address)

main:
    mov rdi, 1   # `1` is stored in RDI as the first argument
    mov rsi, 2   # `2` is stored in RSI as the second argument
    call plus    # call plus function (push the next instruction address to the stack, jump to the plus function address)
    ret          # return from main function
