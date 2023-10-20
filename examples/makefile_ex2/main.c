#include "calc.h"
#include <stdlib.h>

int main()
{

    FILE *fp = fopen("./result.txt", "a");
    CALCULATOR myCalc = {"Basic", 0, fp};

    printf("%f", add(&myCalc, 5, 10));
    fclose(fp);
}

float add(CALCULATOR *c, float x, float y)
{

    char snum[20];
    c->ops_performed += 1;
    float result = x + y;

    itoa(result, snum, 10);

    fwrite(snum, 1, sizeof(snum), c->output);

    return result;
}

float sub(CALCULATOR *c, float x, float y)
{

    char snum[20];
    c->ops_performed += 1;
    float result = x - y;

    itoa(result, snum, 10);

    fwrite(snum, 1, sizeof(snum), c->output);

    return result;
}

float mult(CALCULATOR *c, float x, float y)
{

    char snum[20];
    c->ops_performed += 1;
    float result = x * y;

    itoa(result, snum, 10);

    fwrite(snum, 1, sizeof(snum), c->output);

    return result;
}

// The stdlib library contains a div function, 
// if I named this function div then we would
// have a definition conflict!
float divd(CALCULATOR *c, float x, float y)
{

    char snum[20];
    c->ops_performed += 1;
    float result = x / y;

    itoa(result, snum, 10);

    fwrite(snum, 1, sizeof(snum), c->output);

    return result;
}