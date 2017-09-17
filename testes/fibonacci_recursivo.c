int fib(int x) {
	int a;
	int b;
    if (x == 0)
        return 0;

    if (x == 1)
        return 1;

	a = fib(x-1);
	b = fib(x-2);

    return  a+b ;
}

void main(void) {
	int n;
	n = input();
	output(fib(n), 2);
}
