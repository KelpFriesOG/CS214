#include <stdio.h>

void printnums(int[], int);
void printchars(char*, int);

int main(){

    int nums[10] = {0, 2, 4, 6, 8};
    int bad_nums[] = {1, 3, 5, 7, 9};
    char letters[26];

    for(int i = 0; i < 26; i++){
        letters[i] = i + 65;
    }

    printf("array of nums: \n");
    printnums(nums, 10);
    printf("\narray of chars: \n");
    printchars(letters, 26);

}

void printnums(int* nums, int length){

    for(int i = 0; i < length; i++){
        printf("%d, ", nums[i]);
    }

}

void printchars(char* chars, int length){

    for(int i = 0; i < length; i++){
        printf("%c, ", *(chars + i));
    }

}