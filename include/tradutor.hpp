#ifndef TRADUTOR_HPP
#define TRADUTOR_HPP

#include <bits/stdc++.h>

using namespace std;

class tradutor {
    public:
        tradutor();
        ~tradutor();
        vector<string> traduz_para_ia32(void);
        void escreve_codigo_traduzido(vector<string>);
        bool isNumber(const string& );
};
#endif