#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int i=0;
    int size=1;
    char *pntName=NULL;//a pointer to an array of chars.
    char *temp=NULL;//a temporary pointer

    pntName = malloc( size + 1);//allocate space for two char one for the '\0' to terminate the string
    while(1){
        size++;
        temp = realloc(pntName, size + 1);//reallocat space.
        if ( temp == NULL) {
            printf ( "error allocating memory");
            free ( pntName);
            return 1;
        }
        pntName = temp;
        if ( ( scanf("%c",&pntName[i])) == 1) {
            i++;
            pntName[i] = '\0'; // terminate the string
            if ( pntName[i-1] == ':') {
                break;
            }
        }
        else {
            break;
        }
    }

    printf ( "\n%s\n", pntName);
    printf("Blah...");
    free ( pntName);// release memory
    return 0;
}