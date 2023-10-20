#include <stdio.h>

void foo(int);
void foo2(int*);

int main(){

    int a = 10;

    int * x = &a;


    foo2(x);

    // foo2(&a);

    printf("%d", a);

}

void foo(int x){

    x = x + 5;

}

void foo2(int* y){
    
    *y = *y + 5;

}