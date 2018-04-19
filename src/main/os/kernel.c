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
int PROC_STACK_SIZE[10];
int PROC_STACK_POINTER[10];
int PROC_GLOBAL_POINTER[10];
int PROC_PAGINA_MEM_INST[10];
int PROC_PAGINA_MEM_DADOS[10];
int PROC_QTD_PAGINAS_MEM_INST[10];
int PROC_QTD_PAGINAS_MEM_DADOS[10];

int PROTAGONISTA;
int PROC_ATUAL;

int QUEUE_PRONTO[10];
int IS_EXECUCAO_PREEMPTIVA;

/*******************************************************************************************************/
/*******************************************   MEMORIA   ***********************************************/
/*******************************************************************************************************/

/**
	GERENCIAMENTO DE MEMORIA (Memoria de instruções)
	- Paginação
	- Gerenciamento de memória com partições de tamanho fixo
		São 128 partições de tamanho 32 cada.
*/

// Constantes de gerenciamento da memória
int PARTICOES_MEM_INST[128];						// Partições da memória de instruções
int TAMANHO_PARTICAO;								// Tamanho da partição
int QUANTIDADE_PARTICOES;							// Quantidade de partições
int ERRO_DE_PARTICAO;								// Código de erro
int MAX_PROGRAMAS;									// Número máximo de programas
int ENDERECO_INICIO_HD;								// Endereço de início do HD (Após a área do Kernel)
int ENDERECO_FIM_HD;								// Endereço de fim do HD (Tamanho total)

int STACK_AREA_INICIO;
int STACK_PROG_INICIO;
int STACK_FIM;

int PARTICOES_MEM_DATA[128];						// Partições da memória de dados

// Códigos das interrupções
int INTERRUPT_CONTEXTO;
int INTERRUPT_INPUT;

int PROGRAMAS_EM_HD[10];							// Programas armazenados em disco
int PROGRAMAS_EM_HD_ENDERECO[10];					// Endereço do início do programa no disco
int PROGRAMAS_EM_HD_NOME[10];                       // Nome do programa no disco
int PROGRAMAS_EM_MEMORIA[10];						// Programas carregados na memória de instruções
int PROGRAMAS_EM_MEMORIA_ENDERECO[10];				// Endereço do início do programa na memória de instruções

// OPCODE's
int SYSCALL;										// OPCODE da instrução SYSCALL
int JTM;											// OPCODE da instrução JTM (Jump To Main)
int HALT;											// OPCODE da instrução HALT

// Estados dos menus do display LCD
int KERNEL_MAIN_MENU;
int KERNEL_MENU_HD;
int KERNEL_MENU_HD_DEL;
int KERNEL_MENU_HD_REN;
int KERNEL_MENU_HD_REN_NOME;
int KERNEL_MENU_HD_ADD_NOME;
int KERNEL_MENU_HD_ADD_QUIT;
int KERNEL_MENU_HD_ADD_FIRST_PART;
int KERNEL_MENU_HD_ADD_SECOND_PART;
int KERNEL_MENU_HD_ADD_THIRD_PART;
int KERNEL_MENU_HD_ADD_FOURTH_PART;
int KERNEL_MENU_MEM;
int KERNEL_MENU_MEM_LOAD;
int KERNEL_MENU_MEM_DEL;
int KERNEL_MENU_EXE;
int KERNEL_MENU_EXEC_N_PREEMPTIVO;
int KERNEL_MENU_EXEC_BLOCKED;

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

/**
 * Passa os registradores do processo atual para a área de memória onde o contexto consegue
 * ler. (Consegue retormar contexto dos registradores).
 */
void loadRegistradores(void) {
	int var;
	int pagina;
	int indexVar;
	int indexMemory;
	
	pagina = PROC_PAGINA_MEM_DADOS[PROC_ATUAL] + 1;
	indexVar = pagina * TAMANHO_PARTICAO;
	indexMemory = TAMANHO_PARTICAO * (QUANTIDADE_PARTICOES - 1);

	// Passa os registradores para a área de memoria onde o contexto consegue ler
	while (indexMemory < TAMANHO_PARTICAO * QUANTIDADE_PARTICOES) {
		var = ldm(indexVar);
		sdm(var, indexMemory);
		indexVar += 1;
		indexMemory += 1;
	}
}

/**
 * Salva os registradores em uso na página de memória de registradores do processo em questão.
 * 
 * @param pagina
 * 		pagina de memória de dados do processo que se quer salvar
 */
void saveRegistradores(int pagina) {
	int var;
	int indexVar;
	int indexMemory;

	// Processo bloqueado enquanto era executado em modo preemptivo, salva o contexto para uso posterior.
	pagina += 1;
	indexVar = TAMANHO_PARTICAO * (QUANTIDADE_PARTICOES - 1);
	indexMemory = pagina * TAMANHO_PARTICAO;

	while (indexVar < TAMANHO_PARTICAO * QUANTIDADE_PARTICOES) {
		var = ldm(indexVar);
		sdm(var, indexMemory);
		indexVar += 1;
		indexMemory += 1;
	}
}

int getIdProgramaByName(int programa) {
    int i;
	i = 0;

	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_HD_NOME[i] == programa) {
			return PROGRAMAS_EM_HD[i] - 1;              // Subtrai 1 do id do programa pois a inserção de dados não é 0-based
		}
		i += 1;
	}
}

/**
 * Mata um processo, ou seja, remove todos seus recursos e apontadores.
 * TODO:: REMOVER AREA DE MEMORIA DE DADOS
 * 
 * @param processo
 * 		processo a ser removido da memória de instruções e de dados
 */
void killProcess(int processo) {
	int i;
	int paginas;

	PROC_ATUAL = processo - 1;
	i = PROC_PAGINA_MEM_INST[PROC_ATUAL];
	paginas = PROC_QTD_PAGINAS_MEM_INST[PROC_ATUAL];
	while (paginas > 0) {
		PARTICOES_MEM_INST[i] = 0;
		i += 1;
		paginas -= 1;
	}
	// Elimina a referência da memória
	PROGRAMAS_EM_MEMORIA[PROC_ATUAL] = 0;
	
	// Elimina o restante das referências
	PROC_ESTADO[PROC_ATUAL] = 0;
	PROC_PC[PROC_ATUAL] = 0;
	PROC_STACK_SIZE[PROC_ATUAL] = 0;
	PROC_STACK_POINTER[PROC_ATUAL] = 0;
	PROC_GLOBAL_POINTER[PROC_ATUAL] = 0;
	PROC_PAGINA_MEM_DADOS[PROC_ATUAL] = 0;
	PROC_QTD_PAGINAS_MEM_INST[PROC_ATUAL] = 0;
	PROC_QTD_PAGINAS_MEM_DADOS[PROC_ATUAL] = 0;
	PROC_ATUAL = 0;
	PROTAGONISTA = 999;
}

/**
 * Remove um programa do disco rígido. Não será mais possível carregá-lo em memória.
 * 
 * @param programa
 * 		programa a ser removido do disco
 */
void purgarPrograma(int programa) {
	int index;
	int instrucao;

	programa = getIdProgramaByName(programa);

    // Se o programa estiver na memória, remove-o
    if (PROC_ESTADO[programa] != 0) {
        killProcess(programa + 1);
    }

	index = PROGRAMAS_EM_HD_ENDERECO[programa];
	instrucao = ldk(index);
	while(instrucao >> 26 != SYSCALL) {
		sdk(0, index);
		index += 1;
		instrucao = ldk(index);
	}
	sdk(0, index);

    PROGRAMAS_EM_HD[programa] = 0;
    PROGRAMAS_EM_HD_NOME[programa] = 0;
	PROGRAMAS_EM_HD_ENDERECO[programa] = 0;
}

/**
 * Renomeia um programa armazenado no disco rígido.
 * 
 * @param programa
 *      programa a ser renomeado
 */
void renomearPrograma(int programa) {
    int novoNome;
    
    programa -= 1;
	lcd(KERNEL_MENU_HD_REN_NOME);
    novoNome = input();
    PROGRAMAS_EM_HD_NOME[programa] = novoNome;
}

/**
 * Obtém o descritor dos programas do disco rígido.
 */
int getDescritorProgramasHD(void) {
	int i;
	int descritor;

	i = 0;
	descritor = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_HD[i] != 0) {
			descritor += powByTwo(PROGRAMAS_EM_HD_NOME[i] - 1);
		}
		i += 1;
	}
	return descritor;
}

/**
 * Obtém o descritor dos programas da memória de instruções.
 */
int getDescritorProgramasMemoria(void) {
	int i;
	int descritor;

	i = 0;
	descritor = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_MEMORIA[i] != 0) {
			descritor += powByTwo(PROGRAMAS_EM_HD_NOME[i] - 1);
		}
		i += 1;
	}
	return descritor;
}

/**
 * Obtém o descritor dos programas em estado bloqueado.
 */
int getDescritorProgramasBloqueados(void) {
	int i;
	int descritor;

	i = 0;
	descritor = 0;
	while (i < MAX_PROGRAMAS) {
		if (PROC_ESTADO[i] == BLOQUEADO) {
			descritor += powByTwo(PROGRAMAS_EM_HD_NOME[i] - 1);
		}
		i += 1;
	}
	return descritor;
}

/*******************************************************************************************************/
/*****************************   INICIALIZAÇÃO DO SISTEMA OPERACIONAL   ********************************/
/*******************************************************************************************************/

/**
 * Inicializa todos códigos das possíveis interrupções do sistema.
 */ 
void initInterrupcoes(void) {
	INTERRUPT_CONTEXTO = 2;
	INTERRUPT_INPUT = 1;
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
		PROC_STACK_SIZE[i] = 0;
		PROC_STACK_POINTER[i] = 0;
		PROC_GLOBAL_POINTER[i] = 0;
		PROC_PAGINA_MEM_DADOS[i] = 0;
		PROC_QTD_PAGINAS_MEM_INST[i] = 0;
		PROC_QTD_PAGINAS_MEM_DADOS[i] = 0;
		
		// Filas de processos
		QUEUE_PRONTO[i] = 0;
		i += 1;
	}
	IS_EXECUCAO_PREEMPTIVA = 0;
}

/**
 * Inicializa constantes relacionadas ao gerenciamento de memória.
 */
void initMemoria(void) {
	TAMANHO_PARTICAO = 32;			// Se alterar aqui, lembrar de alterar target.c (endereço para salvar contexto)
	QUANTIDADE_PARTICOES = 128;		// Se alterar aqui, lembrar de alterar target.c (endereço para salvar contexto)
	ERRO_DE_PARTICAO = 100;
	MAX_PROGRAMAS = 10;
	PROTAGONISTA = 999;
}

/**
 * Inicializa constantes relacionadas ao gerenciamento do SHELL-LCD.
 */
void initDisplay(void) {
	// Estados dos menus
	KERNEL_MAIN_MENU = 0;
	KERNEL_MENU_HD = 1;
	KERNEL_MENU_HD_DEL = 2;
    KERNEL_MENU_HD_REN = 3;
	KERNEL_MENU_HD_REN_NOME = 4;
	KERNEL_MENU_HD_ADD_NOME = 5;
	KERNEL_MENU_HD_ADD_QUIT = 6;
	KERNEL_MENU_HD_ADD_FIRST_PART = 7;
	KERNEL_MENU_HD_ADD_SECOND_PART = 8;
	KERNEL_MENU_HD_ADD_THIRD_PART = 9;
	KERNEL_MENU_HD_ADD_FOURTH_PART = 10;
	KERNEL_MENU_MEM = 11;
	KERNEL_MENU_MEM_LOAD = 12;
	KERNEL_MENU_MEM_DEL = 13;
	KERNEL_MENU_EXE = 14;
	KERNEL_MENU_EXEC_N_PREEMPTIVO = 15;
	KERNEL_MENU_EXEC_BLOCKED = 16;

	PROG_INSERT = 30;
	// Atribui o estado inicial do SHELL durante a execução do SO
	ESTADO_LCD = KERNEL_MAIN_MENU;
}

/**
 * Inicializa o constantes diversas do sistema.
 */
void initConstantes(void) {
	// ÚLTIMO ENDEREÇO DO HD
	ENDERECO_FIM_HD = 4095;

	// OPCODES
	SYSCALL = 57;
	JTM = 61;
	HALT = 63;
}

/**
 * Inicializa, primeiramente, todas as partições da memória de
 * instruções como disponíveis. Em seguida, calcula as partições
 * necessárias para armazenar o kernel e as marca como ocupadas.
 * 
 * Também marca as partições da memória de dados utilizadas pelo Kernel
 * e marca a última partição da memória de dados como usada, isso porque
 * é nela que é salvo/lido o contexto de um programa.
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

	// Calcula a quantidade de partições necessárias para o Kernel na memória de INSTRUÇÕES
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

	// Calcula a quantidade de partições necessárias para o Kernel na memória de DADOS
	particoes = gsp() / TAMANHO_PARTICAO;
	if (gsp() % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}
	particoes += 1;

	// Marca as partições em uso pelo Kernel na memória de DADOS
	i = 0;
	while (i < particoes) {
		PARTICOES_MEM_DATA[i] = 1;
		i += 1;
	}

	STACK_AREA_INICIO = particoes * TAMANHO_PARTICAO;
	// Marca a última partição de dados como usada (Para salvar e carregar contextos)
	PARTICOES_MEM_DATA[QUANTIDADE_PARTICOES - 1] = 1;
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
        PROGRAMAS_EM_HD_NOME[prog] = 0;
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
            PROGRAMAS_EM_HD_NOME[prog] = PROGRAMAS_EM_HD[prog];
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
	initInterrupcoes();
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
 * @return primeira partição livre na memória de intruções
 */
int getParticaoLivreMemInstrucoes(int tamanho) {
	int i;
	int particoes;
	int particaoInicial;

	// Calcula quantas partições serão necessárias
	particoes = tamanho / TAMANHO_PARTICAO;
	if (tamanho % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}

	PROC_QTD_PAGINAS_MEM_INST[PROC_ATUAL] = particoes;

	i = 0;
	while (i < QUANTIDADE_PARTICOES) {
		if (PARTICOES_MEM_INST[i] == 0) {
			particaoInicial = i;
			while (particoes > 0) {
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

int getParticaoLivreMemDados(void) {
	int i;
	int particoes;
	int particaoInicial;

	// Calcula quantas partições serão necessárias
	particoes = (PROC_STACK_SIZE[PROC_ATUAL] / TAMANHO_PARTICAO) + 1; // +1 dos registradores
	if (PROC_STACK_SIZE[PROC_ATUAL] % TAMANHO_PARTICAO > 0) {
		particoes += 1;
	}

	i = QUANTIDADE_PARTICOES - 1;
	while (i > 0) {
		if (PARTICOES_MEM_DATA[i] == 0) {
			while (particoes > 0) {
				particaoInicial = i;
				PARTICOES_MEM_DATA[i] = 1;
				particoes -= 1;
				i -= 1;
			}
			return particaoInicial;
		}
		i -= 1;
	}

	return ERRO_DE_PARTICAO;
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

void removeFromQueuePronto(int processo) {
	int i;
	int j;
	i = 0;
	
	// Remove o processo da fila de pronto
	while (i < MAX_PROGRAMAS) {
		if (QUEUE_PRONTO[i] == processo) {
			QUEUE_PRONTO[i] = 0;
			j = i + 1;
		}
		i += 1;
	}

	// Reorganiza a fila
	while (j < MAX_PROGRAMAS) {
		QUEUE_PRONTO[j-1] = QUEUE_PRONTO[j];
		j += 1;
	}

	QUEUE_PRONTO[MAX_PROGRAMAS-1] = 0;
}

void organizeQueuePronto(void) {
	int i;
	i = 1;
	
	while (i < MAX_PROGRAMAS) {
		QUEUE_PRONTO[i-1] = QUEUE_PRONTO[i];
		i += 1;
	}
	QUEUE_PRONTO[MAX_PROGRAMAS-1] = 0;
}

int dequeuePronto(void) {
	int processo;

	if (QUEUE_PRONTO[0] != 0) {
		processo = QUEUE_PRONTO[0];
		organizeQueuePronto();
		return processo;
	}
	return LISTA_VAZIA;
}

void enqueuePronto(int processo) {
	int i;
	i = 0;

	while (i < MAX_PROGRAMAS) {
		if (QUEUE_PRONTO[i] == 0) {
			QUEUE_PRONTO[i] = processo;
			return;
		}
		i += 1;
	}
}

void carregarTodosFilaPronto(void) {
	int i;
	i = 0;
	
	while (i < MAX_PROGRAMAS) {
		if (PROGRAMAS_EM_MEMORIA[i] != 0) {
			if (PROC_ESTADO[i] != BLOQUEADO) {
				PROC_ESTADO[i] = PRONTO;
				enqueuePronto(i+1);
			}
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
	int indexDisk;										// Iterador para o disco
	int indexMemory;									// Iterador para a memória
	int instrucao;										// Instrução lida do disco
	int particao;										// Partição na memória
	int tamanho;										// Tamanho do programa
	int beginOnDisk;									// Endereço de início do programa no HD
	int nProgramaOnDisk;								// Número do programa no HD

    nPrograma = getIdProgramaByName(nPrograma);
	PROC_ATUAL = nPrograma;

	beginOnDisk = PROGRAMAS_EM_HD_ENDERECO[nPrograma];
	nProgramaOnDisk = PROGRAMAS_EM_HD[nPrograma];

	indexDisk = beginOnDisk;							// Recebe o endereço para iterar no disco
	tamanho = getTamanhoPrograma(beginOnDisk);
	particao = getParticaoLivreMemInstrucoes(tamanho);	// Obtém partições livres na memória
	PROC_PAGINA_MEM_INST[PROC_ATUAL] = particao;		// Salva a partição inicial da memória de instruções utilizada pelo processo
	indexMemory = TAMANHO_PARTICAO * particao;			// Endereço para iterar na memória
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
	PROC_ATUAL = 0;
}

/**
 * Só suporta até 32 instruções no max
 */
void criarPrograma(void) {
    int prog;
    int indexDisk;
    int instrucao;
    int opcao;

    indexDisk = ENDERECO_INICIO_HD;

	lcd(KERNEL_MENU_HD_ADD_NOME);
    prog = input();
	prog -= 1;
	PROGRAMAS_EM_HD[prog] = prog + 1; // Vetor continua 0-based, só muda o numero do programa que é +1
    PROGRAMAS_EM_HD_NOME[prog] = PROGRAMAS_EM_HD[prog];
	PROGRAMAS_EM_HD_ENDERECO[prog] = indexDisk;

    // Jump to Main
    instrucao = JTM;
    instrucao <<= 26;
    instrucao += 1;
    sdk(instrucao, indexDisk);
    indexDisk += 1;
    
	lcd(KERNEL_MENU_HD_ADD_QUIT);
    opcao = input();
    while (opcao != 0) {
		lcd(KERNEL_MENU_HD_ADD_FIRST_PART);
        instrucao = input();
        
		lcd(KERNEL_MENU_HD_ADD_SECOND_PART);
		instrucao <<= 8;
        instrucao += input();
        
		lcd(KERNEL_MENU_HD_ADD_THIRD_PART);
		instrucao <<= 8;
        instrucao += input();
        
		lcd(KERNEL_MENU_HD_ADD_FOURTH_PART);
		instrucao <<= 8;
        instrucao += input();
        sdk(instrucao, indexDisk);
        indexDisk += 1;
		
		lcd(KERNEL_MENU_HD_ADD_QUIT);
        opcao = input();
    }

    // Syscall
    // 1ª parte
    instrucao = 127;
    instrucao <<= 8;
    // 2ª parte
    instrucao += 32;
    instrucao <<= 17;
    instrucao >>= 1;
    sdk(instrucao, indexDisk);
}

void run(int programa) {
	if (PROGRAMAS_EM_MEMORIA[PROC_ATUAL] != 0) {
		lcdCurr(programa);
		lcd(PROG_INSERT);

		STACK_PROG_INICIO = STACK_AREA_INICIO + 1;
		PROC_ESTADO[PROC_ATUAL] = EXECUTANDO;
		PROC_PC[PROC_ATUAL] = 0;
		sspb(STACK_PROG_INICIO);
		//sgpb(0);
		
		exec(programa);
		killProcess(programa);
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

	STACK_PROG_INICIO = STACK_AREA_INICIO + 1;
	PROC_ESTADO[PROC_ATUAL] = EXECUTANDO;
	
	pagina = PROC_PAGINA_MEM_DADOS[PROC_ATUAL];
	indexVar = STACK_PROG_INICIO;
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
	sspb(STACK_AREA_INICIO + PROC_STACK_SIZE[PROC_ATUAL]);
	//sgpb(0);

	// execAgain() já realiza a leitura dos registradores do novo contexto, não precisa chamar loadRegs().
	execAgain(PROC_ATUAL + 1, PROC_PC[PROC_ATUAL]);
	killProcess(programa);
	lcd(KERNEL_MAIN_MENU);	
}

/*******************************************************************************************************/
/*************************************   SISTEMA OPERACIONAL   *****************************************/
/*******************************************************************************************************/

void chooseAndRunProtagonista(int programa) {
	PROC_ATUAL = getIdProgramaByName(programa);
    programa = PROC_ATUAL + 1;
	PROTAGONISTA = PROC_ATUAL;
	loadRegistradores();
	runAgain(programa);
}

void runNaoPreemptivo(int programa) {
	PROC_ATUAL = getIdProgramaByName(programa);
    programa = PROC_ATUAL + 1;
	PROTAGONISTA = PROC_ATUAL;
	run(programa);
}

/**
 * Executa todos processos com estado PRONTO da fila de processos, um em seguida do outro.
 */
void runPreemptivo(void) {
	int processo;
	// Coloca todos processos em memória na fila de execução
	carregarTodosFilaPronto();

	// Enquanto tiver processos na fila, executa o próximo
	processo = dequeuePronto();
	while (processo != LISTA_VAZIA) {
		PROC_ATUAL = processo - 1;

		if (PROC_PC[PROC_ATUAL] == 0) {
			run(processo);
		} else {
			loadRegistradores();
			runAgain(processo);
		}
		
		processo = dequeuePronto();
	}
	IS_EXECUCAO_PREEMPTIVA = 0;
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
	if (interrupcao == INTERRUPT_INPUT) {
		STACK_FIM = gspb();
		tamanhoStack = STACK_FIM - STACK_PROG_INICIO + 1;
		PROC_STACK_SIZE[PROC_ATUAL] = tamanhoStack;
		PROC_ESTADO[PROC_ATUAL] = BLOQUEADO;
		PROC_PC[PROC_ATUAL] = gip(); // Salva o pc
		
		if (PROC_PAGINA_MEM_DADOS[PROC_ATUAL] == 0) {
			pagina = getParticaoLivreMemDados();
			PROC_PAGINA_MEM_DADOS[PROC_ATUAL] = pagina;
		} else {
			pagina = PROC_PAGINA_MEM_DADOS[PROC_ATUAL];
		}
		
		indexVar = STACK_PROG_INICIO;
		indexMemory = pagina * TAMANHO_PARTICAO;

		// Salva toda a área de memória stack utilizada pelo processo
		while (tamanhoStack > 0) {
			var = ldm(indexVar);
			sdm(var, indexMemory);
			indexVar += 1;
			indexMemory += 1;
			tamanhoStack -= 1;
		}

		if (PROC_ATUAL == PROTAGONISTA) {
			// Processo bloqueado enquanto era executado em modo não preemptivo, ou seja, é o processo protagonista. Logo, é retomada a execução.
			runAgain(PROC_ATUAL + 1);
		} else {
			saveRegistradores(pagina);
			PROC_ESTADO[PROC_ATUAL] = BLOQUEADO;
			removeFromQueuePronto(PROC_ATUAL + 1);

			if (IS_EXECUCAO_PREEMPTIVA == 1) {
				ESTADO_LCD = KERNEL_MAIN_MENU;
				lcd(ESTADO_LCD);
				rgnsp();
				cic();
				runPreemptivo();
			}
		}

		ESTADO_LCD = KERNEL_MAIN_MENU;
		lcd(ESTADO_LCD);
		rgnsp();
		cic();
	} else if (interrupcao == INTERRUPT_CONTEXTO) {
		STACK_FIM = gspb();
		tamanhoStack = STACK_FIM - STACK_PROG_INICIO + 1;
		PROC_STACK_SIZE[PROC_ATUAL] = tamanhoStack;
		PROC_ESTADO[PROC_ATUAL] = BLOQUEADO;
		PROC_PC[PROC_ATUAL] = gip(); // Salva pc
		
		if (PROC_PAGINA_MEM_DADOS[PROC_ATUAL] == 0) {
			pagina = getParticaoLivreMemDados();
			PROC_PAGINA_MEM_DADOS[PROC_ATUAL] = pagina;
		} else {
			pagina = PROC_PAGINA_MEM_DADOS[PROC_ATUAL];
		}
		
		indexVar = STACK_PROG_INICIO;
		indexMemory = pagina * TAMANHO_PARTICAO;

		// Salva toda a área de memória stack utilizada pelo processo
		while (tamanhoStack > 0) {
			var = ldm(indexVar);
			sdm(var, indexMemory);
			indexVar += 1;
			indexMemory += 1;
			tamanhoStack -= 1;
		}

		if (PROC_ATUAL == PROTAGONISTA) {
			// Processo bloqueado enquanto era executado em modo não preemptivo, ou seja, é o processo protagonista. Logo, é retomada a execução.
			runAgain(PROC_ATUAL + 1);
		} else {
			saveRegistradores(pagina);

			PROC_ESTADO[PROC_ATUAL] = PRONTO;
			ESTADO_LCD = KERNEL_MAIN_MENU;
			lcd(ESTADO_LCD);
			rgnsp();
			cic();

			enqueuePronto(PROC_ATUAL + 1);
			runPreemptivo();
		}

		ESTADO_LCD = KERNEL_MAIN_MENU;
		lcd(ESTADO_LCD);
		rgnsp();
		cic();
	}
	
	while (1) {
		novoEstadoLCD = input();

		if (ESTADO_LCD == KERNEL_MAIN_MENU) {
			if (novoEstadoLCD == 1) {
				novoEstadoLCD = KERNEL_MENU_HD;
			} else if (novoEstadoLCD == 2) {
				novoEstadoLCD = KERNEL_MENU_MEM;
			} else if (novoEstadoLCD == 3) {
				novoEstadoLCD = KERNEL_MENU_EXE;
			} else if (novoEstadoLCD == 4) {
				limparDisplays();
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_HD) {
			if (novoEstadoLCD == 1) {
                criarPrograma();
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD == 2) {
				novoEstadoLCD = KERNEL_MENU_HD_REN;
				lcdPgms(getDescritorProgramasHD());
			} else if (novoEstadoLCD == 3) {
				novoEstadoLCD = KERNEL_MENU_HD_DEL;
				lcdPgms(getDescritorProgramasHD());
			} else {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_HD_DEL) {
			if (novoEstadoLCD > 0) {
				purgarPrograma(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		} else if (ESTADO_LCD == KERNEL_MENU_HD_REN) {
            if (novoEstadoLCD > 0) {
				renomearPrograma(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
        } else if (ESTADO_LCD == KERNEL_MENU_MEM) {
			if (novoEstadoLCD == 1) {
				novoEstadoLCD = KERNEL_MENU_MEM_LOAD;
				lcdPgms(getDescritorProgramasHD());
			} else if (novoEstadoLCD == 2) {
				novoEstadoLCD = KERNEL_MENU_MEM_DEL;
				lcdPgms(getDescritorProgramasMemoria());
			} else {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_MEM_LOAD) {
			if (novoEstadoLCD > 0) {
				carregarPrograma(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		} else if (ESTADO_LCD == KERNEL_MENU_MEM_DEL) {
			if (novoEstadoLCD > 0) {
				killProcess(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		} else if (ESTADO_LCD == KERNEL_MENU_EXE) {
			if (novoEstadoLCD == 1) {
				IS_EXECUCAO_PREEMPTIVA = 1;
				runPreemptivo();
				novoEstadoLCD = KERNEL_MAIN_MENU;
			} else if (novoEstadoLCD == 2) {
				lcdPgms(getDescritorProgramasMemoria());
				novoEstadoLCD = KERNEL_MENU_EXEC_N_PREEMPTIVO;
			} else if (novoEstadoLCD == 3) {
				lcdPgms(getDescritorProgramasBloqueados());
				novoEstadoLCD = KERNEL_MENU_EXEC_BLOCKED;
			} else {
				novoEstadoLCD = KERNEL_MAIN_MENU;
			}
		} else if (ESTADO_LCD == KERNEL_MENU_EXEC_N_PREEMPTIVO) {
			if (novoEstadoLCD > 0) {
				IS_EXECUCAO_PREEMPTIVA = 0;
				runNaoPreemptivo(novoEstadoLCD);
			}
			novoEstadoLCD = KERNEL_MAIN_MENU;
		} else if (ESTADO_LCD == KERNEL_MENU_EXEC_BLOCKED) {
			if (novoEstadoLCD > 0) {
				chooseAndRunProtagonista(novoEstadoLCD);
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