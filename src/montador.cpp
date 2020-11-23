#include "montador.hpp"
#include "leitordearquivos.hpp"

montador::montador() {
    setPC(0);
    setACC(0);

    // adicao da tabela de instrucoes
    //  No formato instrucao, opcode
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("ADD", make_pair(1, 2) ));
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("SUB", make_pair(2, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("MULT", make_pair(3, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("DIV", make_pair(4, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("JMP", make_pair(5, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("JMPN", make_pair(6, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("JMPP", make_pair(7, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("JMPZ", make_pair(8, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("COPY", make_pair(9, 3)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("LOAD", make_pair(10, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("STORE", make_pair(11, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("INPUT", make_pair(12, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("OUTPUT", make_pair(13, 2)) );
    Tabela_de_Instrucoes.insert ( pair<string,pair<int, int>>("STOP", make_pair(14, 1)) );

    // tabela de Diretivas
    Tabela_de_Diretivas.insert ( pair<string,int>("SECTION", 0) );
    Tabela_de_Diretivas.insert ( pair<string,int>("SPACE", 1) );
    Tabela_de_Diretivas.insert ( pair<string,int>("CONST", 1) );
    Tabela_de_Diretivas.insert ( pair<string,int>("EQU", 1) );
    Tabela_de_Diretivas.insert ( pair<string,int>("IF", 1) );
}

montador::~montador() {

}

// metodos get

map<string, pair<int, int>> montador::getTabela_de_Instrucoes() {
    return Tabela_de_Instrucoes;
}

map<string, int> montador::getTabela_de_Simbolos() {
    return Tabela_de_Simbolos;
}

map<string, int> montador::getTabela_de_Diretivas() {
    return Tabela_de_Diretivas;
}

int montador::getPC() {
    return PC;
}

int montador::getACC() {
    return ACC;
}

// metodos set

void montador::setPC(int x) {
    PC = x;
}

void montador::setACC(int x) {
    ACC = x;
}

// outros metodos

int montador::insertNewSymbol(string str, int x) {
    /* if successfull return 1, else return -1 */
    if(str.back() == ':') {
        str.pop_back();
    }
    pair<string, int> temp(str, x);
    pair<map<string,int>::iterator,bool> ret;
    ret = Tabela_de_Simbolos.insert(temp);
    if (ret.second==false) {
        return -1;
    }
    return 1;
}

int montador::getSymbol(string str) {
    map<string, int>::iterator it;
    it = Tabela_de_Simbolos.find(str);
    if (it != Tabela_de_Simbolos.end())
        return Tabela_de_Simbolos.find(str)->second;
    return 0;
}

pair<int, int> montador::checkIfInstruction(string str) {
    /* verifica se uma string faz parte da tabela de instrucoes,
     * caso faca, retorna seu opcode, se nao retorna 0
     */
    map<string,pair<int, int>>::iterator it;
    it = Tabela_de_Instrucoes.find(str);
    if (it != Tabela_de_Instrucoes.end())
        return Tabela_de_Instrucoes.find(str)->second;
    return make_pair(0, 0);
    
}

 int montador::checkIfDiretiva(string str) {
    /* verifica se uma string faz parte da tabela de instrucoes,
     * caso faca, retorna seu opcode, se nao retorna 0
     */
    map<string, int>::iterator it;
    it = Tabela_de_Diretivas.find(str);
    if (it != Tabela_de_Diretivas.end())
        return Tabela_de_Diretivas.find(str)->second;
    return -1;
    
}

int montador::checkIfRotulo(string str) {
    int check;
	/* caso o primeiro elemento seja um simbolo */
	if(str.back() == ':')
        return 1;
    return 0;
}

int montador::PrimeiraPassagem() {
    int contador_linha, contador_palavras, verificaRotulo, insereTabelaSimbolo, verificador2;
    int statusText = 1; // esta na diretiva text
    bool textfaltante = false;
    pair<int, int> verificador;
    contador_linha = 1;
    contador_palavras = 0;
    string linha;
    vector<string> tokens;
    ifstream MyFile("preprocess.txt");
    while(getline(MyFile, linha)) {
        if ( !tokens.empty() ) {
			tokens.clear();
		}
        tokens = split(linha);

        verificaRotulo = checkIfRotulo(tokens[0]); // verifica se primeiro token da linha em um rotulo
        if(verificaRotulo==1) {
            insereTabelaSimbolo = insertNewSymbol(tokens[0], contador_palavras);
                if( insereTabelaSimbolo == -1 ) {
                    cout << "ERRO Semantico redefinicao de Simbolo na linha " << contador_linha <<endl;
                    return -1;
                }
                else {
                    verificador = checkIfInstruction(tokens[1]); // verifica elemento seguinte ao rotulo
                    if( verificador.first == 0 ) {
                        /* NAO eh uma instrucao, verifica se eh uma diretiva */
                        verificador2 = checkIfDiretiva(tokens[1]);
                        if( verificador2 == -1 ) {
                            /* Caso em que nao eh nem diretiva nem instrucao */
                            if(tokens[1].back() == ':') {
                                cout << "ERRO Semantico-> Dois rotulos na mesma linha, na linha: " << contador_linha <<endl;
                                return -1;
                            }
                            cout << "ERRO Sintatico-> Nao eh instrucao nem diretiva, na linha: "  << contador_linha << endl;
                            return -1;
                        }
                        else {
                            /* Caso em que eh uma diretiva */
                            if( statusText != 0) {
                                cout << "ERRO Semantico diretiva na secao errada, na linha: " << contador_linha<< endl;
                                return -1;
                            }
                            if(tokens.size() < 2) {
                                cout << "ERRO Sintatico: quantidade de operandos invalida, linha: "  << contador_linha << endl;
                                return -1;
                            }
                            if( tokens[1].compare("CONST") == 0 ) {
                                bool verificadorConst;
                                verificadorConst = isNumber(tokens[2]);
                                if( !verificadorConst ) {
                                    /* const contem letras */
                                    cout << "Erro Sintatico, diretiva const invalida, linha: " << contador_linha << endl;
                                    return -1;
                                }
                            }
                            if( tokens[1].compare("SPACE") == 0) {
                                if(tokens.size() > 3) {
                                    cout << "Erro sintatico: Diretiva space com mais de 1 valor, linha :" << contador_linha << endl;
                                    return -1;
                                }
                            }
                            contador_palavras += verificador2;
                        }
                    }
                    else {
                        /* caso em que eh uma instrucao */
                        if( statusText != 1) {
                            cout << "ERRO Semantico instrucao na secao errada, na linha: " << contador_linha<< endl;
                            return -1;
                        }
                        if(tokens[1].compare("STOP") !=0 and tokens.size() <= 2) {
                            cout << "ERRO Sintatico: quantidade de operandos invalida, linha: "  << contador_linha << endl;
                            return -1;
                        }
                        if(tokens.size() > verificador.second+1) {
                            cout << "ERRO Sintatico: quantidade de operandos invalida, linha: "  << contador_linha << endl;
                            return -1;
                        }
                        contador_palavras += verificador.second;
                    }
                }
        }
        else {
            /* CASO EM QUE PRIMEIRO ELEMENTO DA LINHA NAO EH UM ROTULO */
            verificador = checkIfInstruction(tokens[0]); // verifica elemento seguinte ao rotulo
                    if( verificador.first == 0 ) {
                        /* NAO eh uma instrucao, verifica se eh uma diretiva */
                        verificador2 = checkIfDiretiva(tokens[0]);
                        if( verificador2 == -1 ) {
                            /* Caso em que nao eh nem diretiva nem instrucao */
                            cout << "ERRO Sintatico linha nao inicia com rotulo, instrucao ou diretiva, na linha: " << contador_linha << endl;
                             return -1;
                            }
                        else {
                            /* Caso em que eh uma diretiva */
                            if(tokens.size() < 2) {
                                cout << "ERRO Sintatico: quantidade de operandos invalida, linha: "  << contador_linha << endl;
                                return -1;
                            }
                            if( (tokens[1].compare("TEXT")==0) or  (tokens[1].compare("DATA")==0) ) {
                                if( (tokens[1].compare("TEXT")==0) ) {
                                    if( textfaltante == false)
                                         textfaltante = true;
                                    else {
                                        cout << "Erro Semantico: 2 secoes de TEXT, na linha: " << contador_linha << endl;
                                        return -1;
                                    }    
                                }
                                else
                                    statusText = 0;
                        
                                contador_palavras += verificador2;  
                            }
                                
                            else {
                                cout << "Erro Semantico Secao invalida na linha: " << contador_linha << endl;
                                return -1;
                            }
                        }
                    }
                    else {
                        /* caso em que eh uma instrucao */
                        if( statusText != 1) {
                            cout << "ERRO Semantico instrucao na secao errada, na linha: " << contador_linha<< endl;
                            return -1;
                        }
                        if(tokens.size() <= 1 and tokens[0].compare("STOP") !=0) {
                            cout << "ERRO Sintatico: quantidade de operandos invalida, linha: "  << contador_linha << endl;
                            return -1;
                        }
                         if(tokens.size() > verificador.second) {
                            cout << "ERRO Sintatico: quantidade de operandos invalida, linha: "  << contador_linha << endl;
                            return -1;
                        }
                        verificaOperandosInvalidos(tokens);
                        contador_palavras += verificador.second;
                    }
        }
        contador_linha++;
    }
    // verificar valores
    /*map<string, int> ts;
    ts = getTabela_de_Simbolos();
    for(auto it = ts.cbegin(); it != ts.cend(); ++it)
    {
        cout << it->first << " " << it->second << "\n";
    } 
    MyFile.close(); */
    if(textfaltante == false) {
        cout << "ERRO Semantico Secao texto faltando, na linha: "<< contador_linha << endl;
        return -1;
    }
        
    return 0;
}

void montador::escreveArqFinal(vector<string> texto)
{
    /* escreve somente com diretiva o */
    if( flagPreProcess != 0 ) {
        ofstream TextoPreprocessado(NomeTextoFinal);
        for(int i=0; i<texto.size(); i++) {
            TextoPreprocessado << texto[i] << " ";
        }
        TextoPreprocessado.close();
    }
    else {
        ofstream TextoPreprocessado(NomeTextoFinal);
        TextoPreprocessado << " ";
        TextoPreprocessado.close();
    }
    
}

bool montador::isNumber(const string& str)
{
  char* ptr;
  strtol(str.c_str(), &ptr, 10);
  return *ptr == '\0';
}


void montador::verificaOperandosInvalidos(vector<string> linha)
{
    string instrucao;
    bool verificador;
    if( linha[0].compare("ADD") == 0 )
    {
        verificador = isNumber(linha[1]);
    }
}

int montador::SegundaPassagem()
{
    int contador_linhas = 1;
    int contador_posicao = 0;
    int tokpos, verificaRotulo;
    int insereTabelaSimbolo, simbolo, verificador_diretiva;
    string linha, linhaParaTextoFinal, linha_simbolos;
    vector<string> linhasTextoFinal;
    vector<string> tokens;
    pair<int, int> verificador_Instrucao;
    ifstream MyFile("preprocess.txt");
    while(getline(MyFile, linha)) {
        if ( !tokens.empty() ) {
			tokens.clear();
		}
        tokens = split(linha);
        

        verificaRotulo = checkIfRotulo(tokens[0]); // verifica se primeiro token da linha em um rotulo
        tokpos = 0;
        if(verificaRotulo==1) {
            tokpos = 1;
        }
        /* primeiro elemento eh um rotulo, ignorar */
            verificador_Instrucao = checkIfInstruction(tokens[tokpos]); // verifica elemento seguinte ao rotulo
            verificador_diretiva = checkIfDiretiva(tokens[tokpos]);
            if(verificador_Instrucao.first != 0 ) {
                /* eh uma instrucao */
                 /* para a quantidade de operandos, verificar se os rotulos sao validos */
                linha_simbolos = "";
                for(int j=tokpos+1; j<verificador_Instrucao.second+tokpos; j++) {
                    if(tokens[j].back() == ',') {
                        tokens[j].pop_back();
                    }
                    insereTabelaSimbolo = insertNewSymbol(tokens[j], contador_posicao);
                    if( insereTabelaSimbolo == 1) {
                        cout << "ERRO Semantico NA LINHA: " << contador_linhas << " Declaracao de Rotulo ausente: " << tokens[j]<< endl;
                        return 0;
                    }
                    else {
                        simbolo = getSymbol(tokens[j]);
                        linha_simbolos += to_string(simbolo) + " ";
                    }
                }
                // Todos os operandos estao OK
                contador_posicao += verificador_Instrucao.second;
                if(linha_simbolos.back() == ' ')
                    linha_simbolos.pop_back();
                linhaParaTextoFinal = "";
                if(tokens[tokpos].compare("STOP") == 0)
                    linhaParaTextoFinal += to_string(verificador_Instrucao.first);
                else
                    linhaParaTextoFinal += to_string(verificador_Instrucao.first) + " " + linha_simbolos;
                linhasTextoFinal.push_back(linhaParaTextoFinal);
            }
            else if(verificador_diretiva != -1){
                /* eh uma diretiva */
                if( tokens[tokpos].compare("CONST") == 0) {
                    linhaParaTextoFinal = tokens[(tokpos + 1)];
                    linhasTextoFinal.push_back(linhaParaTextoFinal);
                }
                if ( tokens[tokpos].compare("SPACE") == 0 ) {
                    linhaParaTextoFinal = "OO";
                    linhasTextoFinal.push_back(linhaParaTextoFinal);
                }
            }
            else {
                if(tokens[tokpos].back() == ':') {
                    cout << "ERRO Sintatico-> Dois rotulos na mesma linha, na linha: " << contador_linhas <<endl;
                    return -1;
                }
                cout << "ERRO Sintatico , NAO EH DIRETIVA NEM INSTRUCAO, na linha: "  << contador_linhas << endl;
                
                return -1;
            }
            contador_linhas++;
    }
    MyFile.close();
    escreveArqFinal(linhasTextoFinal);
    return 0;
}