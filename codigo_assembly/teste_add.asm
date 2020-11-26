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

	push RESPONSE
	call escreve_inteiro

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
escreve_inteiro:
	push ebp ;guarda o valor para pegar os parametros
	; edx vai ser usado para imprimir
	mov ebp, esp
	mov edx, dword [ebp+8]
	; esi vai segurar o numero que vai ser impresso
	mov esi, dword[edx]
	mov edx, 0
	cmp esi, 0
	; ebx vai ser o divisor para pegar os algaritmos separados
	jl imprime_negativo
	jmp verifica_digito
calcula_impressao:
	; carrega o valor para pegar o digito mais significativo
	mov eax, esi
	; divide pelo valor de ebx (pegar centena, dezena, unidade)
	div ebx
; eax vai estar armazenando o digito
	push ebx
	push eax
	call imprime_int
; apos imprimir um digito, recupera o valor de eax
	pop eax
; recupera ebx para saber se esta em dezena, unidade...
	pop ebx
; multiplica pela dezena, centena...
	mul ebx
; subtrai o digito ja escrito
	sub esi, eax
; se ja tiver escrito todos os digitos sai
	jz fim
; divide por 10 ebx
	mov eax, ebx
	mov ebx, 10
	div ebx
	mov ebx, eax
	jmp calcula_impressao
imprime_int:
	push ebp ;guarda o valor para pegar os parametros
; edx vai ser usado para imprimir
	mov ebp, esp
	mov edx, dword [ebp+8]
	add edx, 0x30
	mov [imprime_char], edx
	mov eax, 4
	mov ebx, 1
	mov ecx, imprime_char
	mov edx, 1
	int 80h
	pop ebp
	ret
imprime_negativo:
; multiplica esi por -1, para ficar positivo
	mov eax, esi
	mov esi, -1
	imul esi
	mov esi, eax
; imprime o char -
	mov edx, 45
	mov [imprime_char], edx
	mov eax, 4
	mov ebx, 1
	mov ecx, imprime_char
	mov edx, 1
	int 80h
; deve corrigir o edx para 0 para nao ter problemas na multiplicacao
	mov edx, 0
; verifica quantos digitos o numero tem
	jmp verifica_digito
verifica_digito:
	cmp esi, 100
	jae numero_centena
; se nao for maior ou igual comparar com 10
	cmp esi, 10
	jae numero_dezena 
; se nao for so pode ser unidade
	jmp numero_unidade
numero_centena:
	mov ebx, 100
	jmp calcula_impressao
numero_dezena:
	mov ebx, 10
	jmp calcula_impressao
numero_unidade:
	mov ebx, 1
	jmp calcula_impressao
fim:
	mov eax, 4
	mov ebx, 1
	mov ecx, new_line
	mov edx, new_line_size
	int 80h
	pop ebp
ret

section .bss
RESPONSE resb 52
RESPONSE2 resb 52
imprime_char resb 1

section .data
UM dq 1
new_line db 0dh, 0ah
new_line_size EQU $-new_line
