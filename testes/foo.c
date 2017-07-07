int vetor[2];
int a;
int b;
int c;
int foo(void){
    	output(vetor[0], 0);
	output(vetor[1], 1);
	b = vetor[0];
	vetor[0] = vetor[1];
	vetor[1] = b;
}

void main(void){
    	vetor[0] = 13;
    	vetor[1] = 54;
	foo();
    	output(vetor[0], 0);
	output(vetor[1], 1);
}
