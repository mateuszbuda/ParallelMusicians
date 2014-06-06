#define _GNU_SOURCE 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "mpi.h" 
#define ERR(source) (fprintf(stderr,"%s:%d\n",__FILE__,__LINE__),\
                     perror(source),kill(0,SIGKILL),\
                         exit(EXIT_FAILURE))

#define LINE_BUF 64
#define HEAR_DISTANCE 3.0

typedef struct {
	int x;
	int y;	
} pos;

void read_file(char* pos_file, pos** positions, int* count) {
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

	if( (*positions = calloc(*count, sizeof(pos))) == NULL) {
		ERR("calloc");
	}
	i = 0;
	while(fgets(buf, LINE_BUF, file) != NULL) {
		token = strtok(buf, " "); (*positions)[i].x = atoi(token);
		token = strtok(NULL, " ");
		(*positions)[i++].y = atoi(token);
	}
	
close:
	if(fclose(file) == EOF) {
		ERR("fclose");
	}
}

double distance(pos* a, pos* b) {
	return sqrt( (double) ((a->x - b->x) * (a->x - b->x)) + (double)( (a->y - b->y) * (a->y - b->y)) );
}


int coloring(int vertex, int *neighs, int n_cnt)
{
	int c = 0, w;
	int *neighs_cols;
	int i;
	
	if (0 == n_cnt)
		return 1;

	srand(time(0));
	neighs_cols = (int *) calloc(n_cnt, sizeof(int));
	
	while (!c)
	{
		c = 1 + (rand() % (n_cnt + 1));
		
		for (i = 0; i < n_cnt; i++)
			//send c to neighs[i];
		
		for (i = 0; i < n_cnt; i++)
			//neighs_cols[i] = receive from neighs[i]
		
		if ((w = distinct_color(v, neighs_cols, n_cnt) >= 0 && neighs[w] <= vertex)
			v = 0;
	}
}

int distinct_color(int v, int *cols, int n_cnt)
{
	int i;

	for (i = 0; i < n_cnt; i++)
		if (v == cols[i])
			return i;
	
	return -1;
}

int main(int argc, char* argv[]) {
	int rank, size, i, j, k, l, total, neighbours;
	pos* position;
	int count;
	double dist;
	int* index, *edges;

	read_file(argv[2], &position, &count);

	if( (index = (int*) calloc(count, sizeof(int))) == NULL) {
		ERR("calloc");
	}
	
	if( (edges = (int*) calloc(count * (count - 1), sizeof(int))) == NULL) {
		ERR("calloc");
	}
	
	k = 0;
	l = 0; total = 0; for(i = 0; i < count; ++i) { printf("(%d, %d)\n", position[i].x, position[i].y);
		for(j = 0; j < count; ++j) {
			if(j != i) {
				dist = distance(&position[i], &position[j]);
				printf("\t d( (%d, %d), (%d, %d) ) = %f\n", position[i].x, position[i].y, position[j].x, position[j].y, dist);
				if(dist <= HEAR_DISTANCE) {
					edges[k++] = j;
					++total;
				}
			}
		}
		index[l++] = total;
	}
	
	for(i = 0; i < count; ++i) {
		printf("index[%d] = %d\n", i, index[i]);
	}	
	
	printf("-----------------------\n");
	for(i = 0, j = 1, k = 0; i < count * (count - 1); ++i, ++j) {
		printf("edges[%d] = %d\n", i, edges[i]);
		if(j == index[k]) {
			printf("-----------------------\n");
			++k;
		}
	}
	MPI_Comm graph_comm;
	/* Initialize the environment */
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	MPI_Graph_create(MPI_COMM_WORLD, count, index, edges, 0, &graph_comm);
	MPI_Comm_rank(graph_comm, &rank);
	
	MPI_Graph_neighbors_count(graph_comm, rank, &neighbours);
	
	//int round = coloring(rank, neighbours, neighbours_count)
	
	for (i = 1; i <= neighbours; i++)
	{
		if (round == i)
			printf("Jankiel#%d is playing his wonderful song!\n", rank);

		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0) {
		printf("All done!\n");
	}
	MPI_Finalize();
	return 0;
}
