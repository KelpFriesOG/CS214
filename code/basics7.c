#include <stdio.h>
#include <string.h>

typedef struct student{

    char name[32];
    int age;
    float gpa;

} STUDENT;

void setStudent(STUDENT*, char*, int, float);
void printStudent(STUDENT);

int main(){

    STUDENT kelp = {"Kalpesh", 21, 2.9};

    printStudent(kelp);

    // kelp.name = "Dino"; // This line WILL FAIL.

    // Instead we need to set the values using pointers and arrow notation

    setStudent(&kelp, "Dino", 19, 4.0);

    printStudent(kelp);


}

void setStudent(STUDENT* stdptr, char* name, int age, float gpa)
{

    strcpy(stdptr->name, name);
    stdptr->age = age;
    stdptr->gpa = gpa;

}

void printStudent(STUDENT std){

    printf("%s\n", std.name);
    printf("%d\n", std.age);
    printf("%f\n", std.gpa);

}