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
	int MAIOR_ELEMENTO_INICIO;
	int SORT_INICIO;
	int teste;
	int i;
	int particao;

	TAMANHO_PARTICAO = 32;
	MAIOR_ELEMENTO_INICIO = 128;
	SORT_INICIO = 200;

	particao = input();
	index = carregarPrograma(MAIOR_ELEMENTO_INICIO, particao);
	output(index, 0);

	particao = input();
	index = carregarPrograma(SORT_INICIO, particao);
	output(index, 0);

	// Adiciona a MMU
	i = input();
	mmuLowerIM(TAMANHO_PARTICAO * i, 1);

	if (teste == 9999) {
		output(42, 1);
	} else {
		output(77, 1);
		teste = 9999;
	}
	
	exec(0);
}

/**
 * exec(program)
 *
 * Executa o programa 'program' alocado na memória
 * obs: só funciona com variáveis, pois é um JUMPR
 */