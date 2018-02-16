/**
	GERENCIAMENTO DE MEMORIA (Memoria de instruções)
	- Pensando em implementar paginação
	- Gerenciamento de memória com partições de tamanho fixo
		São 32 partições de tamanho 32 cada.
*/

int PARTICOES[32];									// Partições de memória
int TAMANHO_PARTICAO;								// Tamanho da partição
int ERRO_DE_PARTICAO;								// Código de erro

void inicializarParticoes(void) {
	int i;

	// Inicializa o tamanho das partições
	TAMANHO_PARTICAO = 32;
	ERRO_DE_PARTICAO = 100;

	// Primeiro zera todas partições
	i = 0;
	while (i < 32) {
		PARTICOES[i] = 0;
		i += 1;
	}

	// Marca as partições em uso pelo Sistema Operacional
	i = 0;
	while (i < 7) {
		PARTICOES[i] = 1;
		i += 1;
	}
}

void limparDisplays(void) {
	output(0, 0);
	output(0, 1);
	output(0, 2);
}

int obterParticaoLivre(void) {
	int i;
	i = 0;

	while (i < 32) {
		if (PARTICOES[i] == 0) {
			PARTICOES[i] = 1;
			PARTICOES[i+1] = 1;
			PARTICOES[i+2] = 1;
			return i;
		}
		i += 1;
	}

	return ERRO_DE_PARTICAO;
}

/**
 * Carrega um programa do HD para a memória de instruções.
 * 
 * @param beginOnDisk
 * 		endereço do início do programa no HD
 * @param nPrograma
 * 		seletor para o qual o programa será atribuído na MMU
 */
void carregarPrograma(int beginOnDisk, int nPrograma) {
	int indexDisk;									// Iterador para o disco
	int indexMemory;								// Iterador para a memória
	int instrucao;									// Instrução lida do disco
	int particao;									// Partição na memória
	int SYSCALL;									// OPCODE da instrução SYSCALL

	SYSCALL = 37;

	indexDisk = beginOnDisk;						// Recebe o endereço para iterar no disco
	particao = obterParticaoLivre();				// Obtém uma partição livre na memória
	indexMemory = TAMANHO_PARTICAO * particao;		// Endereço para iterar na memória
	instrucao = ldk(indexDisk);
	while(instrucao >> 26 != SYSCALL) {
		sim(instrucao, indexMemory);
		indexDisk += 1;
		instrucao = ldk(indexDisk);
		indexMemory += 1;
	}
	sim(instrucao, indexMemory);

	// Atribui o seletor da MMU
	mmuSelect(nPrograma);
	// Adiciona offset do programa na MMU
	mmuLowerIM(TAMANHO_PARTICAO * particao);

	return indexMemory;
}

void main(void) {
	int PROGRAMA_1; // Fibonacci
	int PROGRAMA_2; // Maior elemento
	int PROGRAMA_3; // Fatorial

	// Inicializa partições de memória
	inicializarParticoes();

	// Endereços de início dos programas no HD
	PROGRAMA_1 = 800;
	PROGRAMA_2 = 850;
	PROGRAMA_3 = 950;

	// Carrega os programas na memória de instruções
	carregarPrograma(PROGRAMA_1, 1);
	carregarPrograma(PROGRAMA_2, 2);
	carregarPrograma(PROGRAMA_3, 3);

	// Executa os programas, um após o outro, sem preempção
	exec(1);
	exec(2);
	exec(3);
}

/**
 * exec(program)
 *
 * Executa o programa @program alocado na memória
 * obs: só funciona com variáveis, pois é um JUMPR
 * 
 * Precisa salvar pc + 1 no registrador $ra
 * Atribuir o seletor da MMU com o valor @program
 * Passar para o modo usuário
 * Saltar para zero para iniciar a execução do programa selecionado em modo usuário
 */

/**
 * mmuLowerIM(offset)
 * 
 * A unidade de gerenciamento de memória (MMU) armazena um estado, que é o programa selecionado.
 * Quando a função mmuLowerIM(offset) é chamada ela atribui o offset ao programa selecionado no
 * mesmo instante.
 * 
 * Ex: Para atribuir um offset novo ao programa 7 recém-carregado para a memória de instruções,
 * devemos fazer os seguintes chamados de instruções:
 * 
 * -	mmuSelect(7);
 * -	mmuLowerIM(256); // 256, número arbitrário (início do programa na IM)
 */