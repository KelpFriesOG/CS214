#include <stdio.h>

int add(int, int);

void foo();

int sub(int a, int b);

int main()
{
    int i;

    for (i = 0; i < 10; ++i)
        foo();
}

int add(int a, int b){
    return a + b;
}

int sub(int alpha, int beta){
    return alpha - beta;
}

void foo()
{
    int a = 10;
    static int sa = 10;

    a = add(a, 5);
    sa = sub(sa, 5);

    printf("a = %d, sa = %d\n", a, sa);
}
