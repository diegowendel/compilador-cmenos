int fibonacci(int n){
    int c;
    int next;
    int first;
    int second;
    first = 0;
    second = 1;
    c = 0;
    while(c <= n){
        if(c <= 1) {
            next = c;
        }else{
            next = first + second;
            first = second;
            second = next; /* Estava second = first */
        }
        c = c + 1;
    }
    return next;
}
void main(void){
  int n;
  n = input();
  output(fibonacci(n), 2);
}
