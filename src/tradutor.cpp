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
    vector<pair<string, int>> section_bss;
    vector<string> section_data, section_data_arg;
    string linha_atual_traduzida;
    string linha;
    vector<string> tokens;
    int flag_rotulo, flag_inputstr=0, flag_inputc=0, flag_outputstr = 0, flag_input_int=0, flag_output_int =0;
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
                        
                        if(tokens.size() >= 3)
                            section_bss.push_back( make_pair(tokens[0], stoi(tokens[2])) );
                        else
                            section_bss.push_back( make_pair(tokens[0], 0) );
                        
                    }
                    else {
                        section_data.push_back(tokens[0]);
                        section_data_arg.push_back(tokens[2]);
                    }
                }
            flag_rotulo = 1;
            
        }
        if( tokens[0+flag_rotulo].compare("SECTION")== 0 and tokens[1+flag_rotulo].compare("TEXT")== 0 ) {
            linha_atual_traduzida += "section .text\n";
            linha_atual_traduzida += "global _start\n";
            linha_atual_traduzida += "_start:";
        }
        if( tokens[0+flag_rotulo].compare("ADD") == 0 ) {
            linha_atual_traduzida += "\tadd eax, [" + tokens[1+flag_rotulo] + "]";
        }
        if( tokens[0+flag_rotulo].compare("SUB") == 0 ) {
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
            // testar
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
            flag_input_int = 1;
            linha_atual_traduzida += "; le o numero em formato de string\n";
            linha_atual_traduzida += "\tmov eax, 3\n";
            linha_atual_traduzida += "\tmov ebx, 0\n";
            linha_atual_traduzida += "\tmov ecx, " + tokens[1+flag_rotulo] + "\n";
            linha_atual_traduzida += "\tmov edx, 5\n"; // tamanho de 4 bytes para um numero inteiro
            linha_atual_traduzida += "\tint 80h\n";
            // O numero vai estar com seu valor na tabelo ASCII, deve ser corrigido
            linha_atual_traduzida += "\tmov esi, 1\n";
            linha_atual_traduzida += "; empurrar eax para ver a quantidade de iteracoes\n";
            linha_atual_traduzida += "\tsub eax, 1\n";
            linha_atual_traduzida += "\tpush eax\n";
            linha_atual_traduzida += "\tpush " + tokens[1+flag_rotulo] +"; empurra a posicao de memoria onde o numero esta armazenado\n";
            linha_atual_traduzida += "\tcall le_inteiro\n";
            linha_atual_traduzida += "; Recupera o numero em inteiro e escreve no lugar certo\n";
            linha_atual_traduzida += "\tmov [" + tokens[1+flag_rotulo] + "], ebx\n";
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
        if( tokens[0+flag_rotulo].compare("C_OUTPUT")== 0 ) {
            flag_outputstr = 1;
            linha_atual_traduzida += "\tpush " + tokens[1+flag_rotulo]+"\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tpush 1\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tcall Escrever_String";
        }
        if( tokens[0+flag_rotulo].compare("C_INPUT")== 0 ) {
            /* input de um numero inteiro */
            flag_inputstr = 1;
            linha_atual_traduzida += "\tpush " + tokens[1+flag_rotulo]+"\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tpush 1\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tcall Ler_String";
        }
        if( tokens[0+flag_rotulo].compare("S_INPUT") == 0 ) {
            /* input de um numero inteiro */
            /* Como eh necessario ter o tamanho como argumento, deve ser adicionada uma constante conntendo esse valor */
            /* Formato : s_input 10, string_teste */
            flag_inputstr = 1;
            linha_atual_traduzida += "\tpush " + tokens[2+flag_rotulo]+"\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tpush " + tokens[1+flag_rotulo] + "\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tcall Ler_String";  
        }

        if( tokens[0+flag_rotulo].compare("S_OUTPUT")== 0 ) {
            flag_outputstr = 1;
            linha_atual_traduzida += "\tpush " + tokens[2+flag_rotulo]+"\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tpush " + tokens[1+flag_rotulo] + "\n"; // guarda o valor do eax
            linha_atual_traduzida += "\tcall Escrever_String";
            
        }
        

                
        if( linha_atual_traduzida.size() != 0 )
            linhasTextoFinal.push_back(linha_atual_traduzida);
    }

    if(flag_outputstr) {
        linha_atual_traduzida = escreve_output_string();
        linhasTextoFinal.push_back(linha_atual_traduzida);
    }

    if(flag_inputstr) {
        linha_atual_traduzida = escreve_input_string();
        linhasTextoFinal.push_back(linha_atual_traduzida);
    }
    if(flag_input_int) {
        linha_atual_traduzida = escreve_input_int();
        linhasTextoFinal.push_back(linha_atual_traduzida);
    }

    // lembrar de no final adicionar .data e .bss
    if( section_bss.size() != 0) {
        linhasTextoFinal.push_back("\nsection .bss");
        for(int i=0; i<section_bss.size(); i++) {
            linha_atual_traduzida = "";
            section_bss[i].first.pop_back();
            linha_atual_traduzida += section_bss[i].first + " resb " + to_string(section_bss[i].second *4 + 4);
            linhasTextoFinal.push_back(linha_atual_traduzida);
        }   
    }
    if(flag_inputstr != 0) {
        // adiciona ao final da secao bss
        linhasTextoFinal.push_back("input_string resb 4");
    }
    if( section_data.size() != 0) {
        linhasTextoFinal.push_back("\nsection .data");
        for(int i=0; i<section_data.size(); i++) {
            linha_atual_traduzida = "";
            section_data[i].pop_back();
            linha_atual_traduzida += section_data[i] + " dw " + section_data_arg[i];
            linhasTextoFinal.push_back(linha_atual_traduzida);
        }   
    }
    if(flag_inputstr != 0) {
        linhasTextoFinal.push_back("read_string_msg1 db 'Foram lidos '");
        linhasTextoFinal.push_back("str1_size EQU $-read_string_msg1");
        linhasTextoFinal.push_back("read_string_msg2 db ' Caracteres', 0dh, 0ah");
        linhasTextoFinal.push_back("str2_size EQU $-read_string_msg2");
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

string tradutor::escreve_output_string(void)
{
    string linha_nova;
    linha_nova = "";
    linha_nova += "Escrever_String:\n";
    linha_nova += "\tpush ebp ;guarda o valor para pegar os parametros\n";
    linha_nova += "\tmov ebp, esp\n";
    linha_nova += "\tmov esi, dword [ebp+12]\n";
    linha_nova += "\tmov edi, [ebp + 8]\n";
    linha_nova += "\tmov eax, 4\n";
    linha_nova += "\tmov ebx, 1\n";
    linha_nova += "\tmov ecx, esi\n";
    linha_nova += "\tmov edx, edi\n"; // tamanho de 4 bytes para um numero inteiro
    linha_nova += "\tint 80h\n";
    linha_nova += "\tpop ebp\n"; // guarda o valor do eax
    linha_nova += "\tret";
    return linha_nova;
}

string tradutor::escreve_input_string(void)
{
    string linha_nova;
    linha_nova = "";
        linha_nova += "Ler_String:\n";
        linha_nova += "\tpush ebp ;guarda o valor para pegar os parametros\n";
        linha_nova += "\tmov ebp, esp\n";
        linha_nova += "\tmov esi, dword [ebp+12]\n";
        linha_nova += "\tmov edi, [ebp + 8]\n";
        linha_nova += "\t ;faz a leitura da string\n";
        linha_nova += "\tmov eax, 3\n";
        linha_nova += "\tmov ebx, 0\n";
        linha_nova += "\tmov ecx, esi\n";
        linha_nova += "\tmov edx, edi\n"; // tamanho de 4 bytes para um numero inteiro
        linha_nova += "\tint 80h\n";
        linha_nova += "\tadd eax, 0x30\n"; //enquanto so funcionar com inteiros de 1 digito
        linha_nova += "\tmov [input_string], eax\n";
        linha_nova += "\t ;imprime quantos caracteres foram lidos\n";
        linha_nova += "\tmov eax, 4\n";
        linha_nova += "\tmov ebx, 1\n";
        linha_nova += "\tmov ecx, read_string_msg1\n";
        linha_nova += "\tmov edx, str1_size\n";
        linha_nova += "\tint 80h\n";
        linha_nova += "\tmov eax, 4\n";
        linha_nova += "\tmov ebx, 1\n";
        linha_nova += "\tmov ecx, input_string\n"; // euqnato funcionar so para 1 digito, depois chamar subrotina
        linha_nova += "\tmov edx, 4\n";
        linha_nova += "\tint 80h\n";
        linha_nova += "\tmov eax, 4\n";
        linha_nova += "\tmov ebx, 1\n";
        linha_nova += "\tmov ecx, read_string_msg2\n";
        linha_nova += "\tmov edx, str2_size\n";
        linha_nova += "\tint 80h\n";
        linha_nova += "\tpop ebp\n"; // guarda o valor do eax
        linha_nova += "\tret";
        return linha_nova;
}

string tradutor::escreve_input_int(void)
{
    string linha_nova;
    linha_nova = "";
    linha_nova += "le_inteiro:\n";
	linha_nova += "\tpush ebp ;guarda o valor para pegar os parametros\n";
	linha_nova += "; deve receber a quantidade de iteracoes em edi\n";
	linha_nova += "\tmov ebp, esp\n";
	linha_nova += "\tmov edx, dword [ebp+8]\n";
	linha_nova += "\tmov edi, dword[ebp+12]\n";
	linha_nova += "; deve receber a quantidade de iteracoes em edi\n";
	
	linha_nova += "; desloca para a leitura do ultimo digito\n";
	linha_nova += "\tsub eax, 1\n";
	linha_nova += "\tadd edx, eax\n";
	
	linha_nova += "; esi vai armazenar se eh centena, dezena ou unidade\n";
linha_nova += "calcula_num:\n";
	linha_nova += "; le o primeiro digito (mais significativo)\n";
	linha_nova += "\tpush edx\n";
	linha_nova += "\tmovzx eax, byte [edx]\n";
	linha_nova += "; verifica se o numero eh negativo\n";
	linha_nova += "\tcmp eax, 0x2d\n";
	linha_nova += "; decidir o que fazer se for negativo\n";
	linha_nova += "\tje troca_negativo\n";
	linha_nova += "\tmov ecx, eax";
	linha_nova += "; transforma para o valor inteiro\n";
	linha_nova += "\tsub ecx, 0x30\n";
	linha_nova += "\tmov eax, ecx\n";
	linha_nova += "\tmul esi\n";
	linha_nova += "; ebx vai armazenar o numero inteiro final\n";
	linha_nova += "\tadd ebx, eax\n";
	linha_nova += "; multiplica esi por 10\n";
	linha_nova += "\tmov eax, 10\n";
	linha_nova += "\tmul esi\n";
	linha_nova += "\tmov esi, eax\n";
	linha_nova += "; recupera o valor da posicao de memoria e ajusta de acordo com a iteracao\n";
	linha_nova += "\tpop edx\n";
	linha_nova += "\tsub edx, 1\n";
	linha_nova += "; verifica se ja fez a quantidade correta de iteracoes\n";
	linha_nova += "\tsub edi, 1\n";
	linha_nova += "\tjz fim_le_inteiro\n";
	linha_nova += "\tjg calcula_num\n";
linha_nova += "fim_le_inteiro:\n";
    linha_nova += "\tpop ebp\n"; // guarda o valor do eax
	linha_nova += "\tret\n";
linha_nova += "troca_negativo:\n";
    linha_nova += "; eh necessario remover um elemento da pilha, para remover corretamente na saida\n";
    linha_nova += "\tpop ebp\n";
	linha_nova += "\tmov eax, -1\n";
	linha_nova += "\timul ebx\n";
	linha_nova += "\tmov ebx, eax\n";
	linha_nova += "\tjmp fim_le_inteiro";
    return linha_nova;
}