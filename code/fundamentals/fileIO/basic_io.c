#include <stdio.h>
#define CHUNK 256
char buf[CHUNK];

int main()
{

	FILE *fp;

	fp = fopen("test.txt", "w+");

	fprintf(fp, "This is writing into the file (hopefully)\n");

	fputs("This is putting stuff into the file or somethin.\n", fp);

	fclose(fp);
}
