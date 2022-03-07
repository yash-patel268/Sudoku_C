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

	for (int x = 0; x < N; x++) {
		int flags[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		for (int y = 0; y < N; y++) {
			if (flags[sudoku[x][y] - 1] == 0){
                flags[sudoku[x][y] - 1] = 1;
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

void *columnChecker() {
	int incorrect = 0;

	for (int column = 0; column < N; column++) {
		int flag[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		for (int row = 0; row < 9; row++) {
			int toCheck = sudoku[row][column];

			if (flag[toCheck - 1] == 0) {
				flag[toCheck - 1] = 1;
			} else {
				incorrect = 1;
				break;
			}

			if (incorrect == 1){
                break;
            }
		}
	}
	if (incorrect == 0) {
		isCorrect[1] = 1;
	}
	return NULL;
}

void *checkSquare(void * squareInfo) {
	int value;

	parameters * params = squareInfo;
	int box_column = params->column;
	int box_row = params->row;

	int error = 0;
	int flags[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int column = 0; column < 3; column++) {
		for (int row = 0; row < 3; row++) {
			value = sudoku[row + box_row][column + box_column];

			if (flags[value - 1] == 1) {
				error = 1;
				break;
			} else {
				flags[value - 1] = 1;
			}
		}

		if (error == 1) {
			break;
		}

		if (error == 0) {
			isCorrect[2] = 1;
		}
	}
	return NULL;
}

void *validate(void) {
		pthread_t rowThread;
		pthread_create(&rowThread, 0, rowChecker, (void *) "Row Checker");

		pthread_t columnThread;
		pthread_create(&columnThread, 0, columnChecker, (void *) "Column Checker");

		pthread_t squareThread[N];
		parameters squares[N];

		int x = 0;
		for (int countcolumn = 0; countcolumn <= 6; countcolumn += 3) {
			for (int countrow = 0; countrow <= 6; countrow += 3) {
				squares[x].column = countcolumn;
				squares[x].row = countrow;
				pthread_create(&squareThread[x], 0, checkSquare, (void *) &squares[x]);
				x++;
			}
		}

		pthread_join(rowThread, 0);
		pthread_join(columnThread, 0);

		for(int x = 0; x < N; x++){
			pthread_join(squareThread[x], 0);
		}

		return NULL;
}