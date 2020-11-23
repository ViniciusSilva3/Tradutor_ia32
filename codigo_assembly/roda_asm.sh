#!/bin/bash
echo "Compilando programa..."
if  nasm -f elf -o $1.o $1.asm && ld -m elf_i386 -o saida $1.o
then
echo "Sucesso na compilacao"
exit 1
fi
