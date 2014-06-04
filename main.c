#define _GNU_SOURCE 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
//#include "mpi.h"

#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
                         exit(EXIT_FAILURE))

#define LINE_BUF 64

typedef struct {
	int x;
	int y;	
} pos;

void read_file(char* pos_file, pos* positions, int* count) {
	FILE* file;
	char buf[LINE_BUF], *token;
	int i;
	
	if( (file = fopen(pos_file, "r")) == NULL) {
		goto close;
	}

	memset(buf, 0, LINE_BUF);
	if(fgets(buf, LINE_BUF, file) == NULL) {
		goto close;
	}
	
	*count = atoi(buf);

	if( (positions = calloc(*count, sizeof(pos))) == NULL) {
		ERR("calloc");
	}
	i = 0;
	while(fgets(buf, LINE_BUF, file) != NULL) {
		token = strtok(buf, " ");
		positions[i].x = atoi(token);
		token = strtok(NULL, " ");
		positions[i++].y = atoi(token);
	}
	
close:
	if(fclose(file) == EOF) {
		ERR("fclose");
	}
}

int main(int argc, char* argv[]) {
	int rank, size;
	pos position;
	int count;
	
	read_file(argv[1], &position, &count);
	/* Initialize the environment */
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	MPI_Finalize();
	return 0;
}
