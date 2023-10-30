#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv){

	// Argument count must include only the C program's name
	// and the name of the file to read.
	if(argc != 2){
		printf("Usage: %s <filename>\n", argv[0]);
		eturn 1;
	}

	FILE *readme = fopen(argv[1], "r");

	if(readme == NULL){
		perror("ERROR: could not resolve file!");
		return 1;
	}

	int c;
	while((c = fgetc(file)) != EOF){
		putchar(c);
	}

	fclose(c);

}

