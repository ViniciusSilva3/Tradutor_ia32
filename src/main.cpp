#include <bits/stdc++.h>
#include "leitordearquivos.hpp"
#include "testes.hpp"
#include "tradutor.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	montador mont1;
	tradutor tr;
	string nomeArquivoFinal, nomeArquivoPre;
	if(argc != 2) {
		printf("Parametros de entrada incorretos!\n");
		return 0;
	}
	//testeMontador(mont1);
	nomeArquivoFinal = argv[1];
	nomeArquivoFinal = nomeArquivoFinal.substr(0, nomeArquivoFinal.find('.'));
	nomeArquivoFinal = nomeArquivoFinal + ".s";
	
	
	EscreveArquivoPreProcessado(argv[1]);
	tr.escreve_codigo_traduzido(tr.traduz_para_ia32());
	//rename("preprocess.txt", nomeArquivoPre.c_str());
	
	return 0;
}
