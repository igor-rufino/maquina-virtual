# maquina-virtual
Máquina Virtual com memória cache escrita em c++

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
