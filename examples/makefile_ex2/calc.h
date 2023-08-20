#include <stdio.h>

typedef struct calc
{

    char version[20];
    int ops_performed;
    FILE *output;

} CALCULATOR;

float add(CALCULATOR *, float, float);

float sub(CALCULATOR *, float, float);

float mult(CALCULATOR *, float, float);

float divd(CALCULATOR *, float, float);