; Setup interupts
.segment "VECTORS"
    .word NMI ; non-maskable interupt
    .word Reset ; reset press interupt