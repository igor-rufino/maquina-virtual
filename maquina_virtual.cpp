/*

Definicao da Maquina Virtual:
- Quantidade de bits: 16 bits

- Tipos de instrucoes: tipo I e tipo R

- Instrucoes: 
	Aritmeticas:
    - MULTIREG - multiplica dois registros
    - SUMCONS - soma o registro com uma constante 
	Logicas:
    - ANDREG - Funcao Logica AND entre dois registros
    - ORREG - Funcao Logica OR entre dois registros

- Quantidade de bits para os registros de operacao:
    OP (op-code) - [0-1] 2 bits
    RD (registro de destino) - [2-6] 5 bits 
    RT (registro fonte 1) - [7-11] 5 bits
    RS (registro fonte 2) - [11-15] 5 bits
    CONST (constante) - [11-15] 5 bits

- Quantidade de registradores:
    $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $s8, $s9, PC.

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int registers[10];
int cache[4] = {NULL, NULL, NULL, NULL};
int cache_index = 0;

struct instruction{
	int OP;
	int RD;
	int RT;
	int RS;
	int CONST;
} inst;

void initRegisters(){
	for (int i=0; i<10; i++){
		registers[i] = i;
	}
}

void showRegisters(){
	cout << endl << "-REGISTERS-" << endl;
	for (int i=0; i<10; i++){
		cout << "$s" << i << " = " << registers[i] << endl;
	}
	cout << endl << "-CACHE-" << endl;
	for (int i=0; i<4; i++){
		cout << cache[i] << endl;
	}
}

int fetch(int instruction_codes[], int PC){
	bool cache_hit = false;
	cout << endl << "-FETCH-" << endl;
	cout << "PC = " << PC << endl;
	cout << "VALOR DA INSTRUCAO = " << instruction_codes[PC] << endl;
	for (int i=0; i<4; i++){
		//cache_hit = false;
		if(instruction_codes[PC] == cache[i]){
			cout << "CACHE HIT" << endl;
			cache_index = i;
			cache_hit = true;
			break;
		}
	}
	if(cache_hit != true){
		for (int i=0; i<4; i++){
			if(cache[i] == 0){
				cout << "CACHE MISS" << endl;
				cache[i] = instruction_codes[PC];
				cache_index = i;
				break;
			} else if(cache[3] != 0) {
				if (cache_index == 3){
					cout << "CACHE MISS" << endl;
					cache[0] = instruction_codes[PC];
					cache_index = 0;
					break;
				} else {
					cout << "CACHE MISS" << endl;
					cache_index = cache_index + 1;
					cache[cache_index] = instruction_codes[PC];
					break;
				}
			}
		}
	}
	return cache[cache_index];
}

bool decode(int instruction_code){
	cout << endl << "-DECODE-"<< endl;
	
	inst.OP = (instruction_code & 0x18000)>>15;
	inst.RD = (instruction_code & 0x7C00)>>10;
	inst.RT = (instruction_code & 0x3E0)>>5;
	
	cout << "OP = " << inst.OP << endl;
	cout << "RD = " << inst.RD << endl;
	cout << "RT = " << inst.RT << endl;
	
	if (inst.OP<3 && inst.OP>=0){
		inst.RS = (instruction_code & 0x1F);
		cout << "Tipo R -> " << "RS = " << inst.RS << endl;
	}
	else if (inst.OP==3) {
		inst.CONST = (instruction_code & 0x1F); 
		cout << "Tipo I -> " << "CONST = " << inst.CONST << endl;
	}
	else {
		cout << "OP code nao existe" << endl;
		return false;
	}
	return true;
}

bool execute(instruction inst){
	cout << endl << "-EXECUTE-" << endl;
	
	int res;
	switch(inst.OP){
	case 0:
		cout << "MULTIREG $s" << inst.RD << " $s" << inst.RT << " $s" << inst.RS << endl;
		res = registers[inst.RT] * registers[inst.RS];
		if (res > 0xFFFF){
			cout << "Limite de 16 bits" << endl;
			return false;
		}
		else registers[inst.RD] = res;
		showRegisters();
		break;
	
	case 1:
		cout << "ANDREG $s" << inst.RD << " $s" << inst.RT << " $s" << inst.RS << endl;
		res = registers[inst.RT] & registers[inst.RS];
		if (res > 0xFFFF){
			cout << "Limite de 16 bits" << endl;
			return false;
		}
		else registers[inst.RD] = res;
		showRegisters();
		break;
	
	case 2:
		cout << "ORREG $s" << inst.RD << " $s" << inst.RT << " $s" << inst.RS << endl;
		res = registers[inst.RT] | registers[inst.RS];
		if (res > 0xFFFF){
			cout << "Limite de 16 bits" << endl;
			return false;
		}
		else registers[inst.RD] = res;
		showRegisters();
		break;
	
	case 3:
		cout << "SUMCONS $s" << inst.RD << " $s" << inst.RT << " " << inst.CONST << endl;
		res = registers[inst.RT] + inst.CONST;
		if (res > 0xFFFF){
			cout << "Limite de 16 bits" << endl;
			return false;
		}
		else registers[inst.RD] = res;		
		showRegisters();
		break;
	
	default:
		return false;
	}
	return true;
}

int main(){
/*
		OP  RD    RT   RS/C
36    = 00 00000 00001 00100 -> MULTIREG    $s0 $s1 $s4
35049 = 01 00010 00111 01001 -> ANDREG    $s2 $s7 $s9
68870 = 10 00011 01000 00110 -> ORREG    $s3 $s8 $s6
99496 = 11 00001 00101 01000 -> SUMCONS $s1 $s5 01000(8d)
99496 = 11 00001 00101 01000 -> SUMCONS $s1 $s5 01000(8d) - instrucao repetida para busca na memoria cache
37    = 00 00000 00001 00100 -> MULTIREG    $s0 $s1 $s5 - instrucao diferente para sobrescrita na memoria cache
38    = 00 00000 00001 00100 -> MULTIREG    $s0 $s1 $s6 - instrucao diferente para sobrescrita na memoria cache
*/
	
	int instruction_codes[10] = {36, 35049, 68870, 99496, 99496, 37, 38, 36, 35049, 99496};
	int PC;
	int IR;
	bool success_decode;

	cout << "-BEGIN-" << endl << endl;
	
	cout << "-INSTRUCOES-" << endl;
	cout << "        OP  RD    RT   RS/C" << endl;
	cout << "36    = 00 00000 00001 00100 -> MULTIREG    $s0 $s1 $s4" << endl;
	cout << "35049 = 01 00010 00111 01001 -> ANDREG    $s2 $s7 $s9" << endl;
	cout << "68870 = 10 00011 01000 00110 -> ORREG    $s3 $s8 $s6" << endl;
	cout << "99496 = 11 00001 00101 01000 -> SUMCONS $s1 $s5 01000" << endl;
	cout << "99496 = 11 00001 00101 01000 -> SUMCONS $s1 $s5 01000" << endl;
	cout << "37    = 00 00000 00001 00100 -> MULTIREG    $s0 $s1 $s5" << endl;
	cout << "38    = 00 00000 00001 00100 -> MULTIREG    $s0 $s1 $s6" << endl;
	
	initRegisters();
	showRegisters();
	
	cout << endl << "---------------------------" << endl;
	
	for (PC=0; PC<10; PC++){
		IR = fetch(instruction_codes, PC);	
		success_decode = decode(IR);
		if (success_decode){
			if(!execute(inst)) break;
		} else break;
		
		cout << endl << "---------------------------" << endl;
	}
	
	cout << endl << "-END-" << endl;
	
	return 0;
}