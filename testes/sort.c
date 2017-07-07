void sort(int num[], int tam){
    int i;
    int j;
    int min;
    int aux;
    i = 0;
    while (i < tam-1){
        min = i;
        j = i + 1;
        while (j < tam){
            if(num[j] < num[min])
                min = j;
            j = j + 1;
        }
        if (i != min){
            aux = num[i];
            num[i] = num[min];
            num[min] = aux;
        }
        i = i + 1;
    }
}
void main(void){
    int vetor[4];
    int i;
    vetor[0] = 9;
    vetor[1] = 6;
    vetor[2] = 8;
    vetor[3] = 7;
    sort(vetor, 4);

    	output(vetor[0], 0);
	output(vetor[1], 0);
	output(vetor[2], 1);
	output(vetor[3], 2);
}
