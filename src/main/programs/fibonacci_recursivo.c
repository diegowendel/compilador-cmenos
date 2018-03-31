int fib(int x) {

    if(x < 2)
	return 1;

    return fib(x - 1) + fib(x - 2);
}

void main(void) {
	int n;
	n = input();
	output(fib(n), 2);
}
