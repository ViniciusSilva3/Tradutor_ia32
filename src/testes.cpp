#include "testes.hpp"
#include "leitordearquivos.hpp"
void testeMontador(montador mt) {
    printf("###################################\n");
    printf("Iniciando testes:\n");
    if( mt.getPC() == 0 && mt.getACC() == 0) {
        printf("\033[1;32m Teste set/get de PC/ACC Passou! \033[0m\n");
    }
    else {
        printf("\033[1;31m Teste set/get de PC/ACC Falhou! \033[0m\n");
    }
    
    // teste de adicao na tabela de simbolos
    int teste;
	teste = mt.insertNewSymbol("asd", 10);
    if( teste == 1)
        printf("\033[1;32m Teste de adicao na tabela de simbolos passou! \033[0m\n");
    else
    {
        printf("\033[1;31m Teste de adicao na tabela de simbolos falhou! \033[0m\n");
    }
    
	teste = mt.insertNewSymbol("asd", 6);
    if( teste == -1)
        printf("\033[1;32m Teste de adicao repetida na tabela de simbolos passou! \033[0m\n");
    else
    {
        printf("\033[1;31m Teste de adicao repetida na tabela de simbolos falhou! \033[0m\n");
    }

    // Testes de verificar a existencia de uma string na tabela de instrucoes
	teste = mt.checkIfInstruction("ADD");
	if( teste == 1)
        printf("\033[1;32m Teste de verificacao na tabela de Instrucoes passou! \033[0m\n");
    else
    {
        printf("\033[1;31m Teste de verificacao na tabela de Instrucoes falhou! \033[0m\n");
    }
	teste = mt.checkIfInstruction("teste");
	if( teste == 0)
        printf("\033[1;32m Teste de verificacao da nao existencia na tabela de Instrucoes passou! \033[0m\n");
    else
    {
        printf("\033[1;31m Teste de verificacao da nao existencia na tabela de Instrucoes falhou! \033[0m\n");
    }

    // teste de formatar as strings
    string str;
    str = formatString("aDd");
    if(str == "ADD") {
         printf("\033[1;32m Teste de verificacao da formatacao de string passou! \033[0m\n");
    }
    else
    {
        printf("\033[1;31m Teste de verificacao da formatacao de string falhou! \033[0m\n");
    }

}
