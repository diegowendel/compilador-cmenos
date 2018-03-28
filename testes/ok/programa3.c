int fatorial(int number) {
    int result;
    result = 1;

    while (number > 0) {
        result *= number;
        number -= 1;
    }

    return result;
}

void main(void) {
    int a;
    int b;
    int number;
    number = input();
    output(fatorial(number), 2);
}
