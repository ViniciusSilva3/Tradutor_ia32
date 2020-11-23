#include "leitordearquivos.hpp"
#include "montador.hpp"
#include <iostream>
#include <fstream>
#include <regex>

string formatString(string str) {
	for(int i=0; i<str.size(); i++)
		str[i] = toupper(str[i]);
	return str;
}

vector<string> split (string str) {
    vector<string> result;
    string item;
	string str1;
	int flag1;
	flag1 = 0;
	regex reg("([a-zA-Z0-9+;\\-\"]([^ \n\r\t]+)?)");
	smatch matches;
	
	while(regex_search(str, matches, reg)) {
		/* se a linha for um comentario */
		if(matches.str(1)[0] == ';') {
			return result;
		}
		if(matches.str(1).size() != 0) {
			// verifica se tem , separando elementos
			size_t found = matches.str(1).find(',');
			if (found!=string::npos) {
				str1 = "";
				for(int l=0; l<found; l++)
					str1 += matches.str(1)[l];
				result.push_back(str1);
				str1 = "";
				for(int l=found+1; l<matches.str(1).size(); l++)
					str1 +=matches.str(1)[l];
				result.push_back(str1);
			}
			else
				result.push_back(matches.str(1));
		}
			
		str = matches.suffix().str(); //remove o primeiro match
	}
    return result;
}

int checkTokens(vector<string> tok, montador mt) {
	int check;
	pair<int, int> check2;
	/* caso o primeiro elemento seja um simbolo */
	if(tok[0].back() == ':') {
		tok[0].pop_back();
		check = mt.insertNewSymbol(tok[0], 0);
		if( check == -1) {
			cout << "error redefinicao de simbolo" << endl;
			return -1; // -1 significa redefinicao de simbolo
		}
	}
	else {
		check2 = mt.checkIfInstruction(tok[0]);
		if( check2.first != 0) {
			/* eh uma instrucao valida, retorna o seu opcode, como aqui eh primeira
			 * passagem, ainda nao faz nada
			 */
		}
		else {
			/* nao eh nem simbolo nem instrucao, eh um erro */
			cout << "error linha nao comeca em simbolo ou instrucao" << endl;
			return -2;
		}
	}
	return 0;
}

map<string, string> checaEqu(vector<string> linha, map<string, string> mapParam) {
	string equ = "EQU";
	/* se a linha nao tem operador EQU */
	/* Nota, para estar certo ele sempre deve estar na 2 posicao da linha */
	pair<map<string, string>::iterator,bool> ret;
	if(linha.size() > 1) {
		if(equ.compare(linha[1]) != 0)
			return mapParam;
		else {
			if(linha[0].back() == ':')
				linha[0].pop_back();

			ret = mapParam.insert(make_pair(linha[0],linha[2]));
			if (ret.second==false) {
				printf("Error sintatico : Redefinicao de Param por EQU\n");
			}
		}
	}
	return mapParam;
}

vector<string> preProcess(string arquivo) {
	int linhaAtual = 0, flag;
	string linha, linhaTextoSaida, linhaEmBranco;
	vector<string> tokens, textoSaida, tokens2;
	map<string, string> verificaEqu;
	map<string,string>::iterator it;
	// read file
	ifstream MyFile(arquivo);
	while(getline(MyFile, linha)) {
		stringstream ss(linha);
		// para verificar tanto '\r' qunato '\n', dependendo do SO o arquivo fica diferente
		getline(ss, linha, '\r');
		linhaAtual++;
		linhaTextoSaida = "";
		flag = 0;
		/* Limpa os 2 vetores que escrevem nas linhas temporarias */
		if ( !tokens.empty() ) {
			tokens.clear();
		}
		if ( !tokens2.empty() ) {
			tokens2.clear();
		}
		// separa a linha em tokens
		tokens = split(linha);
	
		if(tokens.size() < 2) {
			
			/* verifica se tem 0 ou apenas uma palavra por linha
			 * a unica instrucao que pode ter so 1 palavra eh "STOP"
			 */
			
			if(tokens.size() != 0) {
				tokens[0] = formatString(tokens[0]);
				if( tokens[0].compare("STOP") != 0) {
					/* ler ate encontrar a proxima linha que nao esta em branco ou eh comentario*/
					while(getline(MyFile, linhaEmBranco)) {
						
						tokens2 = split(linhaEmBranco);
						
						if(tokens2.size() != 0) {
							for(int k=0; k<tokens2.size(); k++) {
								tokens.push_back(tokens2[k]);
							}
								
							break;
						}
					} // end while
				}
			}
		} // end procura por quebra de linhas
		
		for(int i=0; i<tokens.size(); i++) {
			tokens[i] = formatString(tokens[i]);
		}
		
		verificaEqu = checaEqu(tokens, verificaEqu);
		/* verifica a existencia de um label definido por um EQU na linha */
		for(int j=0; j<tokens.size(); j++) {
			if(j != 0) {
				if( !verificaEqu.empty() ) {
					it = verificaEqu.find(tokens[j]);
						if (it != verificaEqu.end()) {
							/* Econtrou um EQU e nao eh a deficinao dele */
							tokens[j] = it->second;
						}
				}
			}
			/* caso seja a definicao do EQU nao deve escrever a linha */
			else {
				if( !verificaEqu.empty() ) {
					if(tokens[j].back() == ':') {
						tokens[j].pop_back();
						it = verificaEqu.find(tokens[j]);
						if (it != verificaEqu.end()) {
							/* Econtrou um EQU e nao eh a deficinao dele */
							flag = 1;
						}
						else
							tokens[j].push_back(':');
					}
				}
					
			}
			if(tokens[0].compare("IF")==0 && j != 0) {
				flag = 1;
				if(tokens[j].compare("0") == 0) {
					getline(MyFile, linha, '\r');
					break;
				}
			}
			if(j != tokens.size()-1) 
				linhaTextoSaida += tokens[j] + " ";
			else
				linhaTextoSaida += tokens[j];
		}
		if(flag != 1 && linhaTextoSaida.size()!= 0) {
			textoSaida.push_back(linhaTextoSaida);
		}
	}
	MyFile.close();
	return textoSaida;
}

void escreveArquivo(vector<string> texto) {
	ofstream TextoPreprocessado("preprocess.txt");
	for(int i=0; i<texto.size(); i++) {
		TextoPreprocessado << texto[i] << "\n";
	}
	TextoPreprocessado.close();
}

void EscreveArquivoPreProcessado(string str) {
	vector<string> texto;
	texto = preProcess(str);
	escreveArquivo(texto);
}

void test(string str) {
	regex reg("([a-zA-Z0-9]([^ ]+)?)");
	smatch matches;
	while(regex_search(str, matches, reg)) {
		cout << matches.str(1) << endl;
		str = matches.suffix().str(); //remove o primeiro match
	}
}