.global memory_read
.global memory_write
.global memory_copy

memory_read:
    xor %rax, %rax

.loop1:
    vmovups (%rdi, %rax), %ymm0
    add $32, %rax
    cmp %rsi, %rax
    jl .loop1

    ret

memory_write:
    vxorps %ymm0, %ymm0, %ymm0
    xor %rax, %rax

.loop2:
    vmovups %ymm0, (%rdi, %rax)
    add $32, %rax
    cmp %rsi, %rax
    jl .loop2

    ret

memory_copy:
    shr $1, %rsi
    mov %rsi, %rdx
    xor %rax, %rax

.loop3:
    vmovups (%rdi, %rax), %ymm0
    vmovups %ymm0, (%rdi, %rdx)
    add $32, %rax
    add $32, %rdx
    cmp %rsi, %rax
    jl .loop3

    ret
