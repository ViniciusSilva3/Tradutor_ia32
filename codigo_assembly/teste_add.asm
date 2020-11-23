section .text
global _start
_start:
	mov eax, 100
	sub eax, 99
	;mov ecx, 0
	;cmp eax, ecx
	
	;jg rot2
	;jz rot3
	jl rot1
	
rot2:
	mov eax, 4
	mov ebx, 1
	mov ecx, msg1
	mov edx, 13
	int 80h
	jmp stop
rot1:
	mov eax, 4
	mov ebx, 1
	mov ecx, msg2
	mov edx, 13
	int 80h
	jmp stop
rot3:
	mov eax, 4
	mov ebx, 1
	mov ecx, msg3
	mov edx, 13
	int 80h
	jmp stop

stop:
	mov eax, 1
	mov ebx, 0
	int 80h

section .data
msg1 db 'Hello World', 0dH, 0ah
msg2 db 'Hello Worl2', 0dH, 0ah
msg3 db 'Hello Worl3', 0dH, 0ah
section .bss
RESPONSE resb 4 ; tamanho de 1 byte