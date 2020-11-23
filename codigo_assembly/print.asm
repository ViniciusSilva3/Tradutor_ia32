section .text
global _start
_start:

section .data
msg db 'Hello World', 0dH, 0ah
section .bss
RESPONSE resb 4 ; tamanho de 1 byte
