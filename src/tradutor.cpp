#include "tradutor.hpp"
#include "leitordearquivos.hpp"

tradutor::tradutor()
{
    // metodo construtor
}

tradutor::~tradutor()
{
    //  metodo destrutor
}

bool tradutor::isNumber(const string& str)
{
  char* ptr;
  strtol(str.c_str(), &ptr, 10);
  return *ptr == '\0';
}

vector<string> tradutor::traduz_para_ia32()
{
    // o registrado EAX vai ser usado como o acumuludor do assembly inventado
    
    vector<string> linhasTextoFinal;
    vector<string> section_bss;
    vector<string> section_data, section_data_arg;
    string linha_atual_traduzida;
    string linha;
    vector<string> tokens;
    int flag_rotulo;
    ifstream MyFile("preprocess.txt");
    while(getline(MyFile, linha)) {


        linha_atual_traduzida = "";
        flag_rotulo = 0;
        if ( !tokens.empty() ) {
			tokens.clear();
		}
        tokens = split(linha);
        if(tokens[0].back() == ':') {
            // linha contem um rotulo
            if(tokens[1].compare("CONST") != 0 && tokens[1].compare("SPACE") != 0)
                linha_atual_traduzida = tokens[0];
                else {
                    /* se for um rotulo de reserva de memoria, escrever na secao correta */
                    if (tokens[1].compare("SPACE") == 0) {
                        section_bss.push_back(tokens[0]);
                    }
                    else {
                        section_data.push_back(tokens[0]);
                        section_data_arg.push_back(tokens[2]);
                    }
                }
            flag_rotulo = 1;
            
        }
        if( tokens[0+flag_rotulo].compare("SECTION")== 0 and tokens[1+flag_rotulo].compare("TEXT")== 0 ) {
            linha_atual_traduzida += "section .text";
        }
        if( tokens[0+flag_rotulo].compare("ADD")== 0 ) {
            if(isNumber(tokens[1+flag_rotulo]))
                linha_atual_traduzida += "\tadd eax, " + tokens[1+flag_rotulo];
            else
                linha_atual_traduzida += "\tadd eax, [" + tokens[1+flag_rotulo] + "]";
        }
        if( tokens[0+flag_rotulo].compare("SUB")== 0 ) {
            linha_atual_traduzida += "\tsub eax, [" + tokens[1+flag_rotulo]+ "]";
        }
        if( tokens[0+flag_rotulo].compare("DIV")== 0 ) {
            linha_atual_traduzida += "testando com DIV";
        }
        if( tokens[0+flag_rotulo].compare("MULT")== 0 ) {
            linha_atual_traduzida += "testando com MULT";
        }

        if( tokens[0+flag_rotulo].compare("JMP")== 0 ) {
            linha_atual_traduzida += "\tjmp " + tokens[1+flag_rotulo];
        }
        if( tokens[0+flag_rotulo].compare("JMPN")== 0 ) {
            linha_atual_traduzida += "\tjl " + tokens[1+flag_rotulo];
        }
        if( tokens[0+flag_rotulo].compare("JMPP")== 0 ) {
            linha_atual_traduzida += "\tjg " + tokens[1+flag_rotulo];
        }
        if( tokens[0+flag_rotulo].compare("JMPZ")== 0 ) {
            linha_atual_traduzida += "\tjz " + tokens[1+flag_rotulo];
        }
        if( tokens[0+flag_rotulo].compare("COPY")== 0 ) {
            linha_atual_traduzida += "\tmov " + tokens[2+flag_rotulo] +", " +tokens[1+flag_rotulo];
        }
        if( tokens[0+flag_rotulo].compare("LOAD")== 0 ) {
            linha_atual_traduzida += "\tmov eax, [" + tokens[1+flag_rotulo] + "]";
        }
        if( tokens[0+flag_rotulo].compare("STORE")== 0 ) {
            linha_atual_traduzida += "\tmov [" + tokens[1+flag_rotulo] + "], eax";
        }
        if( tokens[0+flag_rotulo].compare("INPUT")== 0 ) {
            /* input de um numero inteiro */
            linha_atual_traduzida += "\tmov eax, 3\n";
            linha_atual_traduzida += "\tmov ebx, 0\n";
            linha_atual_traduzida += "\tmov ecx, " + tokens[1+flag_rotulo] + "\n";
            linha_atual_traduzida += "\tmov edx, 4\n"; // tamanho de 4 bytes para um numero inteiro
            linha_atual_traduzida += "\tint 80h\n";
            // O numero vai estar com seu valor na tabelo ASCII, deve ser corrigido
            linha_atual_traduzida += "\tmov ecx, [" +tokens[1+flag_rotulo]+ "]\n";
            linha_atual_traduzida += "\tsub ecx, 0x30\n";
            linha_atual_traduzida += "\tmov [" + tokens[1+flag_rotulo] +"], ecx";
        }
        if( tokens[0+flag_rotulo].compare("OUTPUT")== 0 ) {
            linha_atual_traduzida += "\tpush eax\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tmov ecx, ["+tokens[1+flag_rotulo]+"]\n";
            linha_atual_traduzida += "\tadd ecx, 0x30\n";
            linha_atual_traduzida += "\tmov ["+tokens[1+flag_rotulo]+"], ecx\n";

            linha_atual_traduzida += "\tmov eax, 4\n";
            linha_atual_traduzida += "\tmov ebx, 1\n";
            linha_atual_traduzida += "\tmov ecx, " + tokens[1+flag_rotulo] +"\n";
            linha_atual_traduzida += "\tmov edx, 4\n"; // tamanho de 4 bytes para um numero inteiro
            linha_atual_traduzida += "\tint 80h\n";
            linha_atual_traduzida += "\tpop eax"; // guarda o valor do eax
        }
        if( tokens[0+flag_rotulo].compare("STOP")== 0 ) {
            linha_atual_traduzida += "\tmov eax, 1\n";
            linha_atual_traduzida += "\tmov ebx, 0\n";
            linha_atual_traduzida += "\tint 80h";
        }

                
        if( linha_atual_traduzida.size() != 0 )
            linhasTextoFinal.push_back(linha_atual_traduzida);
    }
    // lembrar de no final adicionar .data e .bss
    if( section_bss.size() != 0) {
        linhasTextoFinal.push_back("section.bss");
        for(int i=0; i<section_bss.size(); i++) {
            linha_atual_traduzida = "";
            section_bss[i].pop_back();
            linha_atual_traduzida += section_bss[i] + " resb 4";
            linhasTextoFinal.push_back(linha_atual_traduzida);
        }   
    }
    if( section_data.size() != 0) {
        linhasTextoFinal.push_back("section.data");
        for(int i=0; i<section_data.size(); i++) {
            linha_atual_traduzida = "";
            section_data[i].pop_back();
            linha_atual_traduzida += section_data[i] + " dw " + section_data_arg[i];
            linhasTextoFinal.push_back(linha_atual_traduzida);
        }   
    }
    return linhasTextoFinal;
}

void tradutor::escreve_codigo_traduzido(vector<string> texto)
{
    
    ofstream TextoPreprocessado("teste_saida.txt");
    for(int i=0; i<texto.size(); i++) {
        TextoPreprocessado << texto[i] << endl;
    }
    TextoPreprocessado.close();
    
}