void parOuImpar(int n) {
    if(n % 2 == 0)
	output(2, 2);
    else
	output(1, 2);
}

void main(void) {
    int n;
    n = input();
    parOuImpar(n);
}
