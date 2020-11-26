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
	push RESPONSE; empurra a posicao de memoria onde o numero esta armazenado
	call le_inteiro
; Recupera o numero em inteiro e escreve no lugar certo
	mov [RESPONSE], ebx

; le o numero em formato de string
	mov eax, 3
	mov ebx, 0
	mov ecx, RESPONSE2
	mov edx, 5
	int 80h
	mov esi, 1
; empurrar eax para ver a quantidade de iteracoes
	sub eax, 1
	push eax
	push RESPONSE2; empurra a posicao de memoria onde o numero esta armazenado
	call le_inteiro
; Recupera o numero em inteiro e escreve no lugar certo
	mov [RESPONSE2], ebx
	mov eax, [RESPONSE2]
rot:
	mov eax, 1
	mov ebx, 0
	int 80h
le_inteiro:
	push ebp ;guarda o valor para pegar os parametros
; deve receber a quantidade de iteracoes em edi
	mov ebp, esp
	mov edx, dword [ebp+8]
	mov edi, dword[ebp+12]
; deve receber a quantidade de iteracoes em edi
; desloca para a leitura do ultimo digito
	sub eax, 1
	add edx, eax
; esi vai armazenar se eh centena, dezena ou unidade
calcula_num:
; le o primeiro digito (mais significativo)
	push edx
	movzx eax, byte [edx]
; verifica se o numero eh negativo
	cmp eax, 0x2d
; decidir o que fazer se for negativo
	je troca_negativo
	mov ecx, eax; transforma para o valor inteiro
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
	pop ebp
	ret
troca_negativo:
; eh necessario remover um elemento da pilha, para remover corretamente na saida
	pop ebp
	mov eax, -1
	imul ebx
	mov ebx, eax
	jmp fim_le_inteiro

section .bss
RESPONSE resb 52
RESPONSE2 resb 52

section .data
UM dw 1
