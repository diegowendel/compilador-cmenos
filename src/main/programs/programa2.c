int findMax(int vetor[], int tam){
	int i;
	int pos;
	int max;
	i = 0;
	max = 0;

	while(i < tam) {
		if(max < vetor[i]) {
			max = vetor[i];
			pos = i;
		}
		i = i + 1;
	}

	output(max, 1);
}

void main(void){
	int vetor[6];

	vetor[0] = 12;
	vetor[1] = 41;
	vetor[2] = 23;
	vetor[3] = 98;
	vetor[4] = 33;
	vetor[5] = 21;

	findMax(vetor, 6);
}
