#ifndef leitordearquivos_hpp
#define leitordearquivos_hpp

#include <bits/stdc++.h>
#include "montador.hpp"
using namespace std;


string formatString(string);
vector<string> split(string);
vector<string> preProcess(string);
int checkTokens(vector<string>, montador);
void test(string);
void escreveArquivo(vector<string>);
void EscreveArquivoPreProcessado(string);
map<string, string> checaEqu(vector<string>, map<string, string>);
#endif
