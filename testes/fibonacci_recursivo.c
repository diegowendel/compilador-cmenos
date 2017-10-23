int fib(int x) {
    int a;
    if(x < 2)
	return 1;
    else {
	a = fib(x - 1);
	return a + fib(x - 2);
    }
}

void main(void) {
	int n;
	n = input();
	output(fib(n), 2);
}
