int isPrime(int n) {
    int i;

    if (n <= 1)
        return 2;
    if (n <= 3)
        return 1;

    if (n % 2 == 0)
        return 2;
    if (n % 3 == 0)
	return 2;

    i = 5;
    while (i * i <= n) {
        if (n % i == 0)
            return 2;
	if (n % (i + 2) == 0)
	    return 2;
        i = i + 6;
    }

    return 1;
}

void main(void) {
    int number;
    number = input();
    output(isPrime(number), 2);
}
