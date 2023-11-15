#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

	int pipe_fd[2];
	char data[] = "Hello from the producer!"

	if(pipe(pipe_fd) == -1){
		perror("pipe");
		return(EXIT_FAILURE);
	}

	// We don't need the read end
	// because this program will
	// be writing!
	close(pipe_fd[0]);

	write(pipe_fd[1], data, sizeof(data));

	


}
