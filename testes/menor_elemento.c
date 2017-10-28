int findMin(int vetor[], int tam){
	int i;
	int pos;
	int min;
	i = 0;
	min = 99;

	while(i < tam) {
		if(min > vetor[i]) {
			min = vetor[i];
			pos = i;
		}
		i = i + 1;
	}
	
	output(pos, 1);
	output(min, 2);
}

void main(void){
	int vetor[6];

	vetor[0] = 10;
	vetor[1] = 41;
	vetor[2] = 23;
	vetor[3] = 98;
	vetor[4] = 33;
	vetor[5] = 21;

	findMin(vetor, 6);
}
