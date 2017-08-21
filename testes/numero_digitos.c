void main(void) {
    int n;
    int count;
    count = 0;

    n = input();

    while(n != 0) {
        n = n / 10;
        count = count + 1;
    }

    output(count, 2);
}
