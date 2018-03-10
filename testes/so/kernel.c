/**
 * Sistema Operacional para a máquina alvo iZero.
 * 
 * O sistema conta com gerenciamento de processos, gerenciamento de memória, gerenciamento
 * de arquivos e gerenciamento de entrada e saída.
 * 
 * Desenvolvedor: Diego Ferreira.
 */

/*******************************************************************************************************/
/******************************************   PROCESSOS   **********************************************/
/*******************************************************************************************************/

// Estados possíveis de um processo
int EXECUTANDO;
int PRONTO;
int BLOQUEADO;

int LISTA_VAZIA;

/**
 * TABELA DE PROCESSOS
 * 
 * Idealmente essa estrutura seria um array de structs bem definidas. Por limitações
 * da máquina alvo, só é possível trabalhar com vetores e nenhuma estrutura. Logo,
 * Serão implementados diversos arrays, cada um representando um atributo da estrutura
 * e além disso um inteiro para se atribuir o índice atual que o Kernel está acessando
 * em todos vetores.
 */
int PROC_ESTADO[10];
int PROC_PC[10];
int PROC_STACK_POINTER[10];
int PROC_GLOBAL_POINTER[10];
int PROC_STACK_SIZE[10];

int PROC_ATUAL;

/*******************************************************************************************************/
/*******************************************   MEMORIA   ***********************************************/
/*******************************************************************************************************/

/**
	GERENCIAMENTO DE MEMORIA (Memoria de instruções)
	- Paginação
	- Gerenciamento de memória com partições de tamanho fixo
		São 64 partições de tamanho 32 cada.
*/

// Constantes de gerenciamento da memória
int PARTICOES_MEM_INST[64];							// Partições da memória de instruções
int TAMANHO_PARTICAO;								// Tamanho da partição
int QUANTIDADE_PARTICOES;							// Quantidade de partições
int ERRO_DE_PARTICAO;								// Código de erro
int MAX_PROGRAMAS;									// Número máximo de programas
int ENDERECO_INICIO_HD;								// Endereço de início do HD (Após a área do Kernel)
int ENDERECO_FIM_HD;								// Endereço de fim do HD (Tamanho total)

int STACK_INICIO;
int STACK_FIM;

int PARTICOES_MEM_DATA[64];							// Partições da memória de dados

// Códigos das interrupções
int INTERRUPT_NULL;
int INTERRUPT_BLOQUEADO;
int INTERRUPT_CONTEXTO;
int INTERRUPT_INPUT;
int INTERRUPT_RESET;

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
int KERNEL_MENU_EXEC_N_PREEMPTIVO;
int PROG_INSERT;

int ESTADO_LCD;										// Estado LCD - Menu que será mostrado no Display LCD

/*******************************************************************************************************/
/********************************   ROTINAS AUXILIARES (BIBLIOTECA)   **********************************/
/*******************************************************************************************************/

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

/**
 * Limpa os displays de 7 segmentos com o valor Zero.
 */
void limparDisplays(void) {
	output(0, 0);
	output(0, 1);
	output(0, 2);
}

/*******************************************************************************************************/
/*****************************   INICIALIZAÇÃO DO SISTEMA OPERACIONAL   ********************************/
/*******************************************************************************************************/

/**
 * Inicializa todos códigos das possíveis interrupções do sistema.
 */ 
void initInterrupcoes(void) {
	INTERRUPT_NULL = 0;
	INTERRUPT_BLOQUEADO = 1;
	//INTERRUPT_CONTEXTO = 20;
	//INTERRUPT_INPUT = 1;
	//INTERRUPT_RESET = 40;
}

/**
 * Inicializa constantes relacionadas ao gerenciamento de processos.
 */
void initProcessos(void) {
	int i;

	EXECUTANDO = 1;
	PRONTO = 2;
	BLOQUEADO = 3;
	LISTA_VAZIA = 404;

	// Primeiro zera os vetores de programas
	i = 0;
	while (i < 10) {
		PROC_ESTADO[i] = 0;
		PROC_PC[i] = 0;
		PROC_STACK_POINTER[i] = 0;
		PROC_GLOBAL_POINTER[i] = 0;
		PROC_STACK_SIZE[i] = 0;
		i += 1;
	}
}

/**
 * Inicializa constantes relacionadas ao gerenciamento de memória.
 */
void initMemoria(void) {
	TAMANHO_PARTICAO = 32;			// Se alterar aqui, lembrar de alterar target.c (endereço para salvar contexto)
	QUANTIDADE_PARTICOES = 64;		// Se alterar aqui, lembrar de alterar target.c (endereço para salvar contexto)
	ERRO_DE_PARTICAO = 100;
	MAX_PROGRAMAS = 10;
}

/**
 * Inicializa constantes relacionadas ao gerenciamento do SHELL-LCD.
 */
void initDisplay(void) {
	// Estados dos menus
	KERNEL_MAIN_MENU = 0;
	KERNEL_MENU_HD = 1;
	KERNEL_MENU_MEM = 2;
	KERNEL_MENU_EXE = 3;
	KERNEL_MENU_MEM_LOAD = 4;
	KERNEL_MENU_EXEC_N_PREEMPTIVO = 5;
	PROG_INSERT = 10;
	
	// Atribui o estado inicial do SHELL durante a execução do SO
	ESTADO_LCD = KERNEL_MAIN_MENU;
}

/**
 * Inicializa o constantes diversas do sistema.
 */
void initConstantes(void) {
	// ÚLTIMO ENDEREÇO DO HD
	ENDERECO_FIM_HD = 2047;

	// OPCODES
	SYSCALL = 31;
	JTM = 61;
	HALT = 63;
}

/**
 * Inicializa, primeiramente, todas as partições da memória de
 * instruções como disponíveis. Em seguida, calcula as partições
 * necessárias para armazenar o kernel e as marca como ocupadas.
 */
void initParticoes(void) {
	int i;
	int particoes;
	int tamanhoKernel;

	// Primeiro zera todas partições das memórias de DADOS e INSTRUÇÕES
	i = 0;
	while (i < QUANTIDADE_PARTICOES) {
		PARTICOES_MEM_INST[i] = 0;
		PARTICOES_MEM_DATA[i] = 0;
		i += 1;
	}

	// Calcula a quantidade de partições necessárias para o Kerne na memória de INSTRUÇÕES
	tamanhoKernel = getTamanhoKernel();
	ENDERECO_INICIO_HD = tamanhoKernel + 1;
	particoes = tamanhoKernel / TAMANHO_PARTICAO;
	if (tamanhoKernel % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}

	// Marca as partições em uso pelo Kernel na memória de INSTRUÇÕES
	i = 0;
	while (i < particoes) {
		PARTICOES_MEM_INST[i] = 1;
		i += 1;
	}

	// Calcula a quantidade de partições necessárias para o Kerne na memória de DADOS
	particoes = gsp() / TAMANHO_PARTICAO;
	if (gsp() % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}

	// Marca as partições em uso pelo Kernel na memória de DADOS
	i = 0;
	while (i < particoes) {
		PARTICOES_MEM_DATA[i] = 1;
		i += 1;
	}
}

/**
 * Inicializa, primeiramente, todos slots de programas como
 * disponíveis. Em seguida, realiza uma varredura completa pelo
 * disco em busca de programas ainda não marcados e os mapeia
 * com seu número e seu endereço de início no HD.
 */
void initProgramas(void) {
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
 * Inicializa o sistema operacional.
 */
void initKernel(void) {
	// A ordem de inicialização é fundamental! Não alterar!
	initProcessos();
	initMemoria();
	initDisplay();
	initConstantes();
	initParticoes();				// Inicializa partições de memória
	initProgramas();				// Inicializa os vetores de programas e scaneia o HD
}

/*******************************************************************************************************/
/********************************   GERENCIAMENTO DE MEMÓRIA/DISCO   ***********************************/
/*******************************************************************************************************/

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
	while (i < QUANTIDADE_PARTICOES) {
		if (PARTICOES_MEM_INST[i] == 0) {
			particaoInicial = i;
			while (particoes != 0) {
				PARTICOES_MEM_INST[i] = 1;
				particoes -= 1;
				i += 1;
			}
			return particaoInicial;
		}
		i += 1;
	}

	return ERRO_DE_PARTICAO;
}

int getDescritorProgramasMemoria(void) {
	int i;
	int descritor;

	i = 0;
	descritor = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_MEMORIA[i] != 0) {
			descritor += powByTwo(i);
		}
		i += 1;
	}
	return descritor;
}

int getDescritorProgramasHD(void) {
	int i;
	int descritor;

	i = 0;
	descritor = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_HD[i] != 0) {
			descritor += powByTwo(i);
		}
		i += 1;
	}
	return descritor;
}

int getProcessoBloqueado(void) {
	int i;

	i = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROC_ESTADO[i] == BLOQUEADO) {
			return i;
		}
		i += 1;
	}
	return LISTA_VAZIA;
}

int getProcessoPronto(void) {
	int i;

	i = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROC_ESTADO[i] == PRONTO) {
			return i;
		}
		i += 1;
	}
	return LISTA_VAZIA;
}

void carregarTodosFilaPronto(void) {
	int i;

	i = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_MEMORIA[i] != 0) {
			PROC_ESTADO[i] = PRONTO;
		}
		i += 1;
	}
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
void carregarPrograma(int nPrograma) {
	int indexDisk;									// Iterador para o disco
	int indexMemory;								// Iterador para a memória
	int instrucao;									// Instrução lida do disco
	int particao;									// Partição na memória
	int tamanho;									// Tamanho do programa
	int beginOnDisk;								// Endereço de início do programa no HD
	int nProgramaOnDisk;							// Número do programa no HD

	nPrograma -= 1;									// Subtrai 1 de nPrograma pois a inserção de dados não é 0-based

	beginOnDisk = PROGRAMAS_EM_HD_ENDERECO[nPrograma];
	nProgramaOnDisk = PROGRAMAS_EM_HD[nPrograma];

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
	mmuSelect(nProgramaOnDisk);
	// Adiciona offset do programa na MMU
	mmuLowerIM(TAMANHO_PARTICAO * particao);

	// marca
	PROGRAMAS_EM_MEMORIA[nPrograma] = nProgramaOnDisk;
	PROGRAMAS_EM_MEMORIA_ENDERECO[nPrograma] = beginOnDisk;
}

void run(int programa) {
	if (PROGRAMAS_EM_MEMORIA[PROC_ATUAL] != 0) {
		lcdCurr(programa);
		lcd(PROG_INSERT);

		STACK_INICIO = gsp() + 1;

		PROC_ESTADO[PROC_ATUAL] = EXECUTANDO;
		PROC_PC[PROC_ATUAL] = 0;
		exec(programa);

		PROC_ESTADO[PROC_ATUAL] = 0;
		PROC_PC[PROC_ATUAL] = 0;

		lcd(KERNEL_MAIN_MENU);
	}	
}

void runAgain(int programa) {
	int var;
	int pagina;
	int indexVar;
	int indexMemory;
	int tamanhoStack;

	lcdCurr(programa);
	lcd(PROG_INSERT);

	PROC_ESTADO[PROC_ATUAL] = EXECUTANDO;
	
	pagina = 50;
	indexVar = gsp() + 1;
	indexMemory = pagina * TAMANHO_PARTICAO;
	tamanhoStack = PROC_STACK_SIZE[PROC_ATUAL];
	
	while (tamanhoStack > 0) {
		var = ldm(indexMemory);
		sdm(var, indexVar);
		indexVar += 1;
		indexMemory += 1;
		tamanhoStack -= 1;
	}
	
	// Atribui a stack correta para a re-execução do programa
	sspb(gsp() + PROC_STACK_SIZE[PROC_ATUAL]);
	// TODO:: GLOBAL POINTER	

	// execAgain() já realiza a leitura dos registradores do novo contexto, não precisa chamar loadRegs().
	execAgain(PROC_ATUAL + 1, PROC_PC[PROC_ATUAL]);

	PROC_ESTADO[PROC_ATUAL] = 0;
	PROC_PC[PROC_ATUAL] = 0;
	lcd(KERNEL_MAIN_MENU);
}

/*******************************************************************************************************/
/*************************************   SISTEMA OPERACIONAL   *****************************************/
/*******************************************************************************************************/

void runNaoPreemptivo(int programa) {
	PROC_ATUAL = programa - 1;
	run(programa);
}

void runPreemptivo(void) {
	// Coloca todos processos em memória na fila de execução
	carregarTodosFilaPronto();

	// Enquanto tiver processos na fila, executa o próximo
	while (getProcessoPronto() != LISTA_VAZIA) {
		PROC_ATUAL = getProcessoPronto();
		run(PROC_ATUAL + 1);
	}
}

void main(void) {
	int novoEstadoLCD;
	int interrupcao;
	int inicializando;

	int pagina;

	int var;
	int indexVar;
	int indexMemory;
	int tamanhoStack;

	// Salva sempre registradores no início para serem úteis em uma possível troca de contexto
	saveRegs();

	if (inicializando == 0) {
		// Inicializa o Sistema Operacional
		initKernel();
		// Inicializa display LCD
		lcd(KERNEL_MAIN_MENU);
		// Marca como inicializado
		inicializando = 1;
		cic();
	}
	
	interrupcao = gic();
	if (interrupcao == 1) {
		STACK_FIM = gspb();

		pagina = 50;
		indexVar = STACK_INICIO;
		indexMemory = pagina * TAMANHO_PARTICAO;
		tamanhoStack = STACK_FIM - STACK_INICIO + 1;
		PROC_STACK_SIZE[PROC_ATUAL] = tamanhoStack;

		while (tamanhoStack > 0) {
			var = ldm(indexVar);
			sdm(var, indexMemory);
			indexVar += 1;
			indexMemory += 1;
			tamanhoStack -= 1;
		}

		PROC_ESTADO[PROC_ATUAL] = BLOQUEADO;
		PROC_PC[PROC_ATUAL] = gip() + 1; // Salva pc + 1
		runAgain(PROC_ATUAL);
		rgnsp();
		cic();
	}
	
	while (1) {
		novoEstadoLCD = input();
		output(novoEstadoLCD, 0);

		if (ESTADO_LCD == KERNEL_MAIN_MENU) {
			if (novoEstadoLCD > 4) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD == 4) {
				limparDisplays();
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
				lcdPgms(getDescritorProgramasHD());
			} else if (novoEstadoLCD > 3) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD < 1) {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_EXE) {
			if (novoEstadoLCD == 1) {
				runPreemptivo();
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD == 2) {
				lcdPgms(getDescritorProgramasMemoria());
				novoEstadoLCD = KERNEL_MENU_EXEC_N_PREEMPTIVO;
			} else {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_MEM_LOAD) {
			if (novoEstadoLCD > 0) {
				carregarPrograma(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		} else if (ESTADO_LCD == KERNEL_MENU_EXEC_N_PREEMPTIVO) {
			if (novoEstadoLCD > 0) {
				runNaoPreemptivo(novoEstadoLCD);
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