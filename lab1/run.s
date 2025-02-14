.global run_latency
.global run_throughput

run_latency:
    sarl $2, %edi
.loop1:
    vpaddd %xmm0, %xmm1, %xmm0
    vpaddd %xmm0, %xmm1, %xmm0
    vpaddd %xmm0, %xmm1, %xmm0
    vpaddd %xmm0, %xmm1, %xmm0
    decl %edi
    jnz .loop1
    ret

run_throughput:
.loop2:
    decl %edi
    jnz .loop2
    ret
