int fat(int n) {
	if (n == 0)
		return 1;
   	else if (n == 1)
		return 1;
	else
		return fat(n-1) * n;
}

void main(void) {
	int n;
	n = input();
	output(fat(n), 2);
}
