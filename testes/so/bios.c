void main(void) {
	int i;
	int x;

	checkHD();
	checkIM();
	checkDM();

	i = 0;
	while (i < 68) {
		x = ldk(i);
		sim(x, i);
		i += 1;
	}
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
