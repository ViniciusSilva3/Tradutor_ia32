#ifndef MONTADOR_HPP
#define MONTADOR_HPP

#include <bits/stdc++.h>

using namespace std;

class montador {
    private:
    map<string, pair<int, int>> Tabela_de_Instrucoes;
    map<string, int> Tabela_de_Simbolos;
    map<string, int> Tabela_de_Diretivas;
    int PC; // program counter
    int ACC; // acumulador

    public:
    string NomeTextoFinal;
    int flagPreProcess;
    montador(); // construtor
    ~montador(); // destrutor
    // metodos get
    map<string, pair<int, int>> getTabela_de_Instrucoes();
    map<string, int> getTabela_de_Simbolos();
    map<string, int> getTabela_de_Diretivas();
    int getPC();
    int getACC();

    // metodos set
    void setPC(int);
    void setACC(int);

    // other methods
    int getSymbol(string);
    int insertNewSymbol(string, int);
    pair<int, int> checkIfInstruction(string);
    int PrimeiraPassagem();
    int checkIfRotulo(string);
    int checkIfDiretiva(string);
    void escreveArqFinal(vector<string>);
    int SegundaPassagem(void);
    bool isNumber(const string&);
    void verificaOperandosInvalidos(vector<string>);
};

#endif // MONTADOR_HPP