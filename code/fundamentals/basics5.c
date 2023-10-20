#include <stdio.h>

int main(){

    int y = 15;
    int * x = &y;

    int * z = x;

    printf("%d", *z);

}