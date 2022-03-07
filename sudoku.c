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

int checkNum(int row, int column, int num){
	int rowStart = (row/3) * 3;
	int columnStart = (column/3)*3;

	int flag = 1;

	for (int x = 0; x < N; x++) {
		if (sudoku[row][x] == num) {
			flag = 0;
		}
		
		if (sudoku[x][column] == num) {
			flag = 0;
		}

		for (int x = rowStart; x < rowStart+3; x++){
			for (int y = columnStart; y < 3; y++) {
				if (sudoku[x][y] == num ) {
					flag = 0;

				}
			}
		}
	}
	return flag;
}
