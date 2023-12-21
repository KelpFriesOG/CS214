#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


int main(int argc, char **argv)
{
    char *dname = argc > 1 ? argv[1] : ".";
    int dlen = strlen(dname);
    if (dlen > 0 && dname[dlen-1] == '/') dlen--;
    DIR *dirp = opendir(dname);
    if (!dirp) {
	perror(dname);
	exit(EXIT_FAILURE);
    }

    printf("Reading directory %s\n", dname);

    struct dirent *de;
    while ((de = readdir(dirp))) {
	int flen = strlen(de->d_name);
	char *fname = malloc(dlen + 1 + flen + 1);
	memcpy(fname, dname, dlen);
	fname[dlen] = '/';
	memcpy(fname + dlen + 1, de->d_name, flen + 1);

	//puts(fname);

	struct stat sbuf;
	int r = stat(fname, &sbuf);
	if (r < 0) {
	    perror(fname);
	    continue;
	}

	char *ftype = 
	    S_ISDIR(sbuf.st_mode) ? "DIR" : 
	    S_ISREG(sbuf.st_mode) ? "REG" : "???";
	
	printf(" %s %s\n", ftype, fname);
	

	/*
	printf("  %s %s\n", 
	    de->d_type == DT_REG ? "R" :
		de->d_type == DT_DIR ? "D" : "O"
	    , de->d_name);
	*/


	free(fname);
    }

    puts("Complete");
    closedir(dirp);

    return EXIT_SUCCESS;
}
