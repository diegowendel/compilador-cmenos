int fatorial(int number) {
    int result;
   // result = 1;

    while (number > 0 && number < 99 && number < 98) {
        result = result * number;
        number -= number - 1;
    }

    return result;
}

void main(void) {
    int number;

    number = input();
    output(fatorial(number), 2);
}