/**
	GERENCIAMENTO DE MEMORIA (Memoria de instruções)
	- Pensando em implementar paginação
	- Gerenciamento de memória com partições de tamanho fixo
		São 32 partições de tamanho 32 cada.
*/

/*void carregarPrograma(int inicioDisco, int fimDisco) {
	int QTD_PARTICOES;		// Quantidade total de partições
	int TAM_PARTICAO;		// Tamanho da partição
	int index; 				// Usado para iterar nas posições do disco
	int fimParticao; 		// Endereço de fim da partição em memória
	int i;					// Iterador de partições
	int j;					// Iterador de endereços dentro de uma partição
	int instrucao;			// Instrução lida do disco
	int inicioMemoria;
	int HALT;
	
	//QTD_PARTICOES = 32;
	//TAM_PARTICAO = 32;

	/*i = 4; // Três primeiras partições são reservadas ao SO
	index = inicioDisco; // Recebe o endereço para iterar no disco
	inicioMemoria = TAM_PARTICAO * 4;
	//while (i < QTD_PARTICOES) {
		//if (index <= fimDisco) {
			j = TAM_PARTICAO * i;
			//if (particoes[i] == 0) { // Partição livre
				//fimParticao = TAM_PARTICAO * 5;
				
				//while (j < fimParticao) {
				while (index <= 130) {
					instrucao = ldk(index);
					sim(instrucao, j);
					output(index, 2);
					index += 1;
					j += 1;
					output(j, 2);
				}
			//}
		//}
		//i += 1;
	//}*
}*/

void limparDisplays(void) {
	output(0, 0);
	output(0, 1);
	output(0, 2);
}

int carregarPrograma(int beginOnDisk) {
	int instrucao;
	int index;
	int SYSCALL;

	SYSCALL = 36;

	index = beginOnDisk;
	instrucao = ldk(index);
	while(instrucao >> 26 != SYSCALL) {
		sim(instrucao, index);
		index += 1;
		instrucao = ldk(index);
	}
	sim(instrucao, index);
	return index;
}

void main(void) {
	//int particoes[32];
	int index;
	int MAIOR_ELEMENTO_INICIO;
	int SORT_INICIO;
	int teste;
	int i;

	MAIOR_ELEMENTO_INICIO = 99;
	SORT_INICIO = 188;
	index = carregarPrograma(MAIOR_ELEMENTO_INICIO);
	index = carregarPrograma(SORT_INICIO);
	// Adiciona a MMU
	i = input();
	mmuLowerIM(i, 1);

	if (teste == 9999) {
		output(42, 0);
	} else {
		output(77, 0);
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