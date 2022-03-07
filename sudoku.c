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

	int returnValue = 1;

	for (int x = 0; x < N; x++) {
		if (sudoku[row][x] == num) {
		    returnValue = 0;
		}
		
		if (sudoku[x][column] == num) {
			returnValue = 0;
		}

		for (int x = rowStart; x < rowStart+3; x++){
			for (int y = columnStart; y < 3; y++) {
				if (sudoku[x][y] == num ) {
					returnValue = 0;

				}
			}
		}
	}
	return returnValue;
}

int sudokuSolver(int row, int col) {
    if (sudoku[row][col] == 0) {
        for(int x=1; x<=N; x++){
            if (checkNum(row, col, x) == 1){
                
                sudoku[row][col] = x;
    
                if ((row+1) < N) {
                    
                    if (solver(row + 1, col) == 1) {
                        return 1;
                    } else {
                        
                        sudoku[row][col] = 0;
                    }
                
                } else if ((col+1)<9) {
                    if (solver(0, col + 1) == 1) {
                        return 1;
                    } else {
                        
                        sudoku[row][col] = 0;
                    }
                } else { 
                    return 1;
                }
            }
        }
    } else {
        if((row+1) < N) {
            return solver(row +1, col);
        } else if((col+1) < N) {
            return solver(0, col+1);
        } else {
            return 1;
        }
    } 
    return 0;
}

void *rowChecker() {
	int passfail = 1;

	for (int a = 0; a < 9; a++) {
		int flags[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		for (int i = 0; i < 9; i++) {
			if (flags[sudoku[a][i] - 1] == 0){
                flags[sudoku[a][i] - 1] = 1;
            } else {
				passfail = 0;
				break;
			}
		}
	}
	if (passfail == 1) {
		isCorrect[0] = 1;
	}
	return NULL;
}