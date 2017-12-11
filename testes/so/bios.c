void main(void) {
	int i;	

	checkHD();
	checkIM();
	checkDM();

	i = 0;
	while (i < 500) {
		sim(ldk(i), i);
		i += 1;
	}
	i = 0;
}

/**
 * ldk(address)
 *
 * RS - Address
 * RT - Register Destiny
 */

/**
 * sim(data, address)
 * 
 * RS - Address
 * RT - Data
 */
