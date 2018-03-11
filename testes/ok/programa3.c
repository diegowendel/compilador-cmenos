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
    a = 93;
    b = a;
    number = input();
    output(a, 0);
    output(b, 1);
    output(fatorial(number), 2);
}
