#define BUFSIZE 16

typedef struct {
    int bufpos;
    int buflen;
    int fd;
    char buf[BUFSIZE];
} LINES;


LINES *lines_fdinit(int fd);
LINES *lopen(char *path);

void lclose(LINES *);

char *getline(LINES *);

