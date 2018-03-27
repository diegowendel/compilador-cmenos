/**
 * Realiza o teste de partida antes de carregar o SO.
 */
void checarComponentes(void) {
	lcd(20); // Checa HD
	input();
	lcd(21); // Checa memória de instruções
	input();
	lcd(22); // Checa memória de dados
	input();
	lcd(23); // Inicia o Sistema Operacional
	input();
}

/**
 * Transfere as instruções referentes ao SO do HD para a memória de instruções.
 * Retorna a posição da última instrução copiada do HD.
 */
void carregarKernel(void) {
	int instrucao;
	int index;
	int HALT;

	HALT = 63; // Opcode da instrução HALT

	index = 0; // O SO DEVE SEMPRE INICIAR DO ZERO
	instrucao = ldk(index);
	while (instrucao >> 26 != HALT) {
		sim(instrucao, index);
		index += 1;
		instrucao = ldk(index);
	}
	sim(instrucao, index);
}

/**
 * Parte principal da bios. Este programa carrega o Sistema Operacional e realiza algumas
 * checagens de componentes da máquina alvo. Não são adicionados offsets na MMU para o SO
 * pois o mesmo tem prioridade de execução e não necessita de offsets, uma vez que SEMPRE
 * o SO deverá estar armazenado no início da memória de instruções e utilizar também o 
 * início da memória de dados.
 */
void main(void) {		
	checarComponentes();
	carregarKernel();
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

/**
 * mmuLowerIM(data, selector)
 * 
 * RS - Selector
 * RT - Data
 */

/**
 * mmuUpperIM(data, selector)
 * 
 * RS - Selector
 * RT - Data
 */