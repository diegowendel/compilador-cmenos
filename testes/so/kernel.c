/**
	GERENCIAMENTO DE MEMORIA (Memoria de instruções)
	- Paginação
	- Gerenciamento de memória com partições de tamanho fixo
		São 32 partições de tamanho 32 cada.
*/

// Constantes de gerenciamento da memória
int PARTICOES[32];									// Partições de memória
int TAMANHO_PARTICAO;								// Tamanho da partição
int ERRO_DE_PARTICAO;								// Código de erro
int MAX_PROGRAMAS;									// Número máximo de programas
int ENDERECO_INICIO_HD;								// Endereço de início do HD (Após a área do Kernel)
int ENDERECO_FIM_HD;								// Endereço de fim do HD (Tamanho total)

int PROGRAMAS_EM_HD[10];							// Programas armazenados em disco
int PROGRAMAS_EM_HD_ENDERECO[10];
int PROGRAMAS_EM_MEMORIA[10];						// Programas carregados na memória de instruções
int PROGRAMAS_EM_MEMORIA_ENDERECO[10];

// OPCODE's
int SYSCALL;										// OPCODE da instrução SYSCALL
int JTM;											// OPCODE da instrução JTM (Jump To Main)
int HALT;											// OPCODE da instrução HALT

// Estados dos menus do display LCD
int KERNEL_MAIN_MENU;
int KERNEL_MENU_HD;
int KERNEL_MENU_MEM;
int KERNEL_MENU_EXE;
int KERNEL_MENU_MEM_LOAD;
int PROG_INSERT;

int ESTADO_LCD;										// Estado LCD - Menu que será mostrado no Display LCD

/**
 * Inicializa constantes globais do sistema.
 */ 
void inicializarConstantes(void) {
	TAMANHO_PARTICAO = 32;
	ERRO_DE_PARTICAO = 100;
	MAX_PROGRAMAS = 10;
	ENDERECO_FIM_HD = 1023;

	SYSCALL = 31;
	JTM = 61;
	HALT = 63;

	KERNEL_MAIN_MENU = 0;
	KERNEL_MENU_HD = 1;
	KERNEL_MENU_MEM = 2;
	KERNEL_MENU_EXE = 3;
	KERNEL_MENU_MEM_LOAD = 4;	
	PROG_INSERT = 9;
	
	ESTADO_LCD = KERNEL_MAIN_MENU;
}

/**
 * Inicializa, primeiramente, todos slots de programas como
 * disponíveis. Em seguida, realiza uma varredura completa pelo
 * disco em busca de programas ainda não marcados e os mapeia
 * com seu número e seu endereço de início no HD.
 */
void inicializarProgramas(void) {
	int index;
	int prog;
	int instrucao;

	// Primeiro zera os vetores de programas
	prog = 0;
	while (prog < MAX_PROGRAMAS) {
		PROGRAMAS_EM_HD[prog] = 0;
		PROGRAMAS_EM_HD_ENDERECO[prog] = 0;
		PROGRAMAS_EM_MEMORIA[prog] = 0;
		PROGRAMAS_EM_MEMORIA_ENDERECO[prog] = 0;
		prog += 1;
	}

	// Varre o disco em busca de programas
	index = ENDERECO_INICIO_HD;
	prog = 0;
	while (index < ENDERECO_FIM_HD) {
		instrucao = ldk(index);
		if (instrucao >> 26 == JTM) {
			PROGRAMAS_EM_HD[prog] = prog + 1; // Vetor continua 0-based, só muda o numero do programa que é +1
			PROGRAMAS_EM_HD_ENDERECO[prog] = index;
			prog += 1;
		}
		index += 1;
	}
}

/**
 * Calcula o tamanho do programa do kernel em disco.
 *
 * @return tamanho do kernel
 */ 
int getTamanhoKernel(void) {
	int instrucao;
	int index;

	index = 0;
	instrucao = ldk(index);
	while (instrucao >> 26 != HALT) {
		index += 1;
		instrucao = ldk(index);
	}
	return index;
}

/**
 * Inicializa inicialmente todas as partições da memória de
 * instruções como disponíveis. Em seguida, calcula as partições
 * necessárias para armazenar o kernel e as marca como ocupadas.
 */
void inicializarParticoes(void) {
	int i;
	int particoes;
	int tamanhoKernel;

	// Primeiro zera todas partições
	i = 0;
	while (i < 32) {
		PARTICOES[i] = 0;
		i += 1;
	}

	// Calcula a quantidade de partições necessárias para o Kernel	
	tamanhoKernel = getTamanhoKernel();
	ENDERECO_INICIO_HD = tamanhoKernel + 1;
	particoes = tamanhoKernel / TAMANHO_PARTICAO;
	if (tamanhoKernel % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}

	// Marca as partições em uso pelo Kernel
	i = 0;
	while (i < particoes) {
		PARTICOES[i] = 1;
		i += 1;
	}
}

/**
 * Com base no tamanho passado como parâmetro, busca partições livres
 * e contíguas na memória. Retorna o número da primeira partição, mesmo
 * que o programa necessite de mais de uma para ser alocado em memória.
 * 
 * @param
 * 		tamanho do programa
 * @return primeira partição livre na memória
 */
int getParticaoLivre(int tamanho) {
	int i;
	int particoes;
	int particaoInicial;

	// Calcula quantas partições serão necessárias
	particoes = tamanho / TAMANHO_PARTICAO;
	if (tamanho % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}

	i = 0;
	while (i < 32) {
		if (PARTICOES[i] == 0) {
			particaoInicial = i;
			while (particoes != 0) {
				PARTICOES[i] = 1;
				particoes -= 1;
				i += 1;
			}
			return particaoInicial;
		}
		i += 1;
	}

	return ERRO_DE_PARTICAO;
}

/**
 * Calcula o tamanho de um programa em disco.
 * 
 * @param beginOnDisk
 * 		endereço do início do programa no HD
 * @return tamanho do programa
 */ 
int getTamanhoPrograma(int beginOnDisk) {
	int instrucao;
	int index;

	index = beginOnDisk;
	instrucao = ldk(index);
	while (instrucao >> 26 != SYSCALL) {
		index += 1;
		instrucao = ldk(index);
	}
	return index - beginOnDisk;
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
	int tamanho;									// Tamanho do programa

	indexDisk = beginOnDisk;						// Recebe o endereço para iterar no disco
	tamanho = getTamanhoPrograma(beginOnDisk);
	particao = getParticaoLivre(tamanho);			// Obtém partições livres na memória
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
}

/**
 * Realiza a exponenciação de 2, onde o expoente é passado como
 * parâmetro.
 * 
 * @param n
 * 		expoente da operação de exponenciação de 2
 * @return potência de dois do número passado como parâmetro
 */
int powByTwo(int n) {
	int total;
	
	if (n == 0) {
		return 1;
	}

	total = 1;
	while (n > 0) {
		total *= 2;
		n -= 1;
	}
	return total;
}

void main(void) {
	int novoEstadoLCD;
	int i;
	int count;

	// Inicializa display LCD
	lcd(KERNEL_MAIN_MENU);
	// Inicializa constantes gerais usadas pelo SO
	inicializarConstantes();
	// Inicializa partições de memória
	inicializarParticoes();
	// Inicializa os vetores de programas e scaneia o HD
	inicializarProgramas();

	// Loop infinito
	while (1) {
		novoEstadoLCD = input();
		output(novoEstadoLCD, 0);

		if (ESTADO_LCD == KERNEL_MAIN_MENU) {
			if (novoEstadoLCD > 3) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD < 1) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_HD) {
			if (novoEstadoLCD > 3) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD < 1) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_MEM) {
			if (novoEstadoLCD == 1) {
				novoEstadoLCD = KERNEL_MENU_MEM_LOAD;
				i = 0;
				count = 0;

				while (i < MAX_PROGRAMAS) {
					if (PROGRAMAS_EM_HD[i] != 0) {
						output(count, 1);
						count += powByTwo(i);
					}
					i += 1;
				}
				output(count, 2);
			} else if (novoEstadoLCD > 3) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD < 1) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_EXE) {
			if (novoEstadoLCD > 0) {
				lcd(PROG_INSERT);
				exec(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		} else if (ESTADO_LCD == KERNEL_MENU_MEM_LOAD) {
			if (novoEstadoLCD > 1) {
				carregarPrograma(PROGRAMAS_EM_HD_ENDERECO[novoEstadoLCD-1], PROGRAMAS_EM_HD[novoEstadoLCD-1]);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		}

		ESTADO_LCD = novoEstadoLCD;
		lcd(ESTADO_LCD);
	}
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

/**
 * lcd(menu)
 * 
 * Seleciona o menu que será exibido no display LCD.
 */