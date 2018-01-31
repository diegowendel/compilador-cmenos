/**
 * ldk(address)
 *
 * RS - Address
 * RT - Register Destiny
 *
 * returns a instruction of HD
 */

/**
 * sim(data, address)
 * 
 * RS - Address
 * RT - Data
 */

void main(void) {
	int HALT;
	int instrucao;
	int i;
	
	HALT = 24; // Opcode da instrução HALT

	//checkHD();
	//checkIM();
	//checkDM();

	i = 0;
	instrucao = ldk(i);
	while (instrucao >> 26 != HALT) {
		sim(instrucao, i);
		i += 1;
		instrucao = ldk(i);
	}
	sim(instrucao, i);
}
