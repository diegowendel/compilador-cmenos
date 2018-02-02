/**
 * Realiza o teste de partida antes de carregar o SO.
 */
void checarComponentes(void) {
	checkHD(); // Checa HD
	checkIM(); // Checa memória de instruções
	checkDM(); // Checa memória de dados
}

/**
 * Transfere as instruções referentes ao SO do HD para a memória de instruções.
 * Retorna a posição da última instrução copiada do HD.
 */
int carregarSistemaOperacional(void) {
	int instrucao;
	int index;
	int HALT;

	HALT = 24; // Opcode da instrução HALT

	index = 0;
	instrucao = ldk(index);
	while (instrucao >> 26 != HALT) {
		sim(instrucao, index);
		index += 1;
		instrucao = ldk(index);
	}
	sim(instrucao, index);
	return index;
}

void main(void) {
	int lower; // Lower bound
	int upper; // Upper bound
	int index;
		
	// checarComponentes();
	index = carregarSistemaOperacional();

	lower = 0;
	upper = index;
	mmu(lower, upper, 0);
}

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