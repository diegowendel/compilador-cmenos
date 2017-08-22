int isDivisibleByFive(int number) {
    if (number == 0) 
	return 2;

    if (number % 5 == 0)
        return 1;
    else
        return 2;
}

void main(void) {
    int number;
    number = input();
    output(isDivisibleByFive(number), 2);
}
