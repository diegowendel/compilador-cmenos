/**
	GERENCIAMENTO DE MEMORIA (Memoria de instruções)
	- Pensando em implementar paginação
	- Gerenciamento de memória com partições de tamanho fixo
		São 32 partições de tamanho 32 cada.
*/

int TAMANHO_PARTICAO;								// Tamanho da partição

void limparDisplays(void) {
	output(0, 0);
	output(0, 1);
	output(0, 2);
}

int carregarPrograma(int beginOnDisk, int particao) {
	int instrucao;									// Instrução lida do disco
	int indexDisk;									// Iterador para o disco
	int indexMemory;								// Iterador para a memória
	int SYSCALL;									// OPCODE da instrução SYSCALL

	SYSCALL = 36;

	indexDisk = beginOnDisk;						// Recebe o endereço para iterar no disco
	indexMemory = TAMANHO_PARTICAO * particao;		// Endereço para iterar na memória
	instrucao = ldk(indexDisk);
	while(instrucao >> 26 != SYSCALL) {
		sim(instrucao, indexMemory);
		indexDisk += 1;
		instrucao = ldk(indexDisk);
		indexMemory += 1;
	}
	sim(instrucao, indexMemory);
	return indexMemory;
}

void main(void) {
	//int particoes[32];
	int index;
	int PROGRAMA_1; // Fibonacci
	int PROGRAMA_2; // Maior elemento
	int PROGRAMA_3; // Fatorial
	int particao;

	TAMANHO_PARTICAO = 32;
	PROGRAMA_1 = 150;
	PROGRAMA_2 = 200;
	PROGRAMA_3 = 254;

	//particao = 5;
	carregarPrograma(PROGRAMA_1, 5);
	carregarPrograma(PROGRAMA_2, 8);
	carregarPrograma(PROGRAMA_3, 11);

	// Adiciona a MMU
	mmuLowerIM(TAMANHO_PARTICAO * 5, 1);
	exec();
	mmuLowerIM(TAMANHO_PARTICAO * 8, 2);
	exec();
	mmuLowerIM(TAMANHO_PARTICAO * 11, 3);
	exec();
}

/**
 * exec(program)
 *
 * Executa o programa 'program' alocado na memória
 * obs: só funciona com variáveis, pois é um JUMPR
 */