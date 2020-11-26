section .text
global _start
_start:
mov eax, dword[BASE]
imul dword[ALTURA]
cdq
idiv dword[DOIS]
mov dword[RES], eax
sys_exit: mov eax, 1
mov ebx, 0
int 80h
section .bss
RES resd 1
section .data
BASE dd 3
ALTURA dd 4
DOIS dd 2
