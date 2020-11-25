section .text
global _start
_start:
	; le o numero em formato de string
	mov eax, 3
	mov ebx, 0
	mov ecx, RESPONSE
	mov edx, 5
	int 80h
	mov esi, 1
	; empurrar eax para ver a quantidade de iteracoes
	sub eax, 1
	push eax
	push RESPONSE
le_inteiro:
	; deve receber a quantidade de iteracoes em edi
	pop edx
	; desloca para a leitura do ultimo digito
	sub eax, 1
	add edx, eax
	pop edi
	; esi vai armazenar se eh centena, dezena ou unidade
calcula_num:
	; le o primeiro digito (mais significativo)
	push edx
	movzx eax, byte [edx]
	; verifica se o numero eh negativo
	cmp eax, 0x2d
	; decidir o que fazer se for negativo
	je troca_negativo
	mov ecx, eax
	; transforma para o valor inteiro
	sub ecx, 0x30
	mov eax, ecx
	mul esi
	; ebx vai armazenar o numero inteiro final
	add ebx, eax
	; multiplica esi por 10
	mov eax, 10
	mul esi
	mov esi, eax
	; recupera o valor da posicao de memoria e ajusta de acordo com a iteracao
	pop edx
	sub edx, 1
	; verifica se ja fez a quantidade correta de iteracoes
	sub edi, 1
	jz fim_le_inteiro
	jg calcula_num
fim_le_inteiro:
	mov eax, 1
	mov ebx, 0
	int 80h
troca_negativo:
	mov eax, -1
	imul ebx
	jmp fim_le_inteiro



section .bss
RESPONSE resb 20
input_string resb 4

section .data
UM dw 1
read_string_msg1 db 'Foram lidos '
str1_size EQU $-read_string_msg1
read_string_msg2 db ' Caracteres', 0dh, 0ah
str2_size EQU $-read_string_msg2
