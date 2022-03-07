#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 9

int sudoku[N][N];
int isCorrect[3] = {0, 0, 0};

typedef struct {
	int column,row;
} parameters;

void readPuzzle(FILE *file) {
	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			fscanf(file, "%d", &sudoku[x][y]);
		}
	}
}


