void main(void) {
	// Variavel que representa o programa em execução
	int programa;

	// Adiciona endereços dos programas
	addProgramStart(31, 0); // MAIOR ELEMENTO
	addProgramStart(85, 1); // MENOR ELEMENTO
	addProgramStart(139, 2); // SORT
	addProgramStart(210, 3); // FIBONACCI
	addProgramStart(254, 4); // FATORIAL
	//addProgramStart(x, 5);
	//addProgramStart(x, 6);
	//addProgramStart(x, 7);
	//addProgramStart(x, 8);
	//addProgramStart(x, 9);

	// Seleciona um programa para executar

	// ******* FALTA ARRUMAR O LOAD ESPECIAL DE PROGRAMA *******/
	programa = readProgramStart(input());
	output(programa, 0);
	
	exec(programa);
}

/**
 * addProgramStart(start, address);
 * - start [endereço de início do programa na memória de instruções]
 * - address [endereço na memória de dados]
 */

/**
 * exec(program)
 *
 * Executa o programa 'program' alocado na memória
 */
