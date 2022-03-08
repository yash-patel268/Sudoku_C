#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//Define N equal to 9 for later use for loops or array length
#define N 9

//Initialize sudoku array which is 9x9
int sudoku[N][N];
//Initialize array to check if the are correct using formart of "row, column, box"
int isCorrect[3] = {0, 0, 0};

//Define a struct called paramaters that has the column and row parameter
typedef struct {
	int column,row;
} parameters;

//Initialize and define readPuzzle function
void readPuzzle(FILE *file) {
	//loop through rows and columns from puzzle.txt and assign it to sudoku array 
	for(int x = 0; x < N; x++) {
		for(int y = 0; y < N; y++) {
			fscanf(file, "%d", &sudoku[x][y]);
		}
	}
}

//Initialize and define checkNum function
int checkNum(int row, int column, int num){
	//Initialize the start row variable
	int rowStart = (row/3) * 3;
	//Initialize the start column variable
	int columnStart = (column/3)*3;

	//Initialize the returnValue equal to 1
	int returnValue = 1;

	//Loop through each column and row
	for (int x = 0; x < N; x++) {
		//If the column contains the number change returnValue to 0
		if (sudoku[row][x] == num) {
		    returnValue = 0;
		}
		//If the row contains the number change returnValue to 0
		if (sudoku[x][column] == num) {
			returnValue = 0;
		}

		//loop through each square to check if the square contain the number and change returnValue to 0
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

//Initialize and define sudokuSolver function
int sudokuSolver(int row, int col) {
	//If the value at that specific row and column is not solved
    if (sudoku[row][col] == 0) {
		//Loop through possible solutions for 1-9
        for(int x=1; x<=N; x++){
			//check if the number is present
            if (checkNum(row, col, x) == 1){
				//If number doesnt exist place the number
                sudoku[row][col] = x;
				//check next row
                if ((row+1) < N) {
                    //Solve next row and see return is equal to 1
                    if (sudokuSolver(row + 1, col) == 1) {
                        return 1;
					//else set to 0 and try again
                    } else {
                        sudoku[row][col] = 0;
                    }
                //check next column
                } else if ((col+1)<9) {
					//Solve next column and see return is equal to 1
                    if (sudokuSolver(0, col + 1) == 1) {
                        return 1;
					//else set to 0 and try again
                    } else {
                        sudoku[row][col] = 0;
                    }
                } else { 
                    return 1;
                }
            }
        }
	//If the value at that specific column and row isnt 0
    } else {
        if((row+1) < N) {
            return sudokuSolver(row +1, col);
        } else if((col+1) < N) {
            return sudokuSolver(0, col+1);
        } else {
            return 1;
        }
    } 
    return 0;
}

//Initialize and define rowChecker function
void *rowChecker() {
	int passfail = 1;
	//Loop through all  rows
	for (int x = 0; x < N; x++) {
		//Initialize array to hold each value
		int flags[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		//Loop through all columns
		for (int y = 0; y < N; y++) {
			//if the flag at that specific value has not been used in that row
			if (flags[sudoku[x][y] - 1] == 0){
				//change to one
                flags[sudoku[x][y] - 1] = 1;
			//if value has been used then break out of loop
            } else {
				passfail = 0;
				break;
			}
		}
	}

	//If all values are solved 
	if (passfail == 1) {
		isCorrect[0] = 1;
	}
	return NULL;
}

//Initialize and define columnChecker function
void *columnChecker() {
	int incorrect = 0;

	//Loop through all columns
	for (int column = 0; column < N; column++) {
		//Initialize array to hold each value
		int flag[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		//Loop through all rows
		for (int row = 0; row < 9; row++) {
			//Initialize var to hold value of sudoku at certian row and column
			//check to see if value doesn't occur already
			int toCheck = sudoku[row][column];
			//if value doesnt doesnt occur in column	
			if (flag[toCheck - 1] == 0) {
				flag[toCheck - 1] = 1;
			//if value occurs break out of loop
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

//Initialize and define checkSquare function
void *checkSquare(void * squareInfo) {
	//initialize var to hold value at specific column and row
	int value;
	//initialize var using paramaters struct that holds
	parameters * params = squareInfo;
	//initialize var to the column parameter
	int box_column = params->column;
	//initialize var to the row parameter
	int box_row = params->row;

	//initialixe var to hold if there is an error
	int error = 0;
	//initialize array to hold all value within the square
	int flags[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//Loop through all columns within the square
	for (int column = 0; column < 3; column++) {
		//Loop through all rows within the square
		for (int row = 0; row < 3; row++) {
			value = sudoku[row + box_row][column + box_column];
			//If value has been found assign error to 1
			if (flags[value - 1] == 1) {
				error = 1;
				break;
			//If value wasnt found set that flag equal to one
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

//Initialize and define validate function
void *validate(void) {
		//Initialize thread for rows
		pthread_t rowThread;
		//Create thread using rowChecker function
		pthread_create(&rowThread, 0, rowChecker, (void *) "Row Checker");
		//Initialize thread for columns
		pthread_t columnThread;
		//Create thread using columnChecker function
		pthread_create(&columnThread, 0, columnChecker, (void *) "Column Checker");

		//Initialize thread array for all squares
		pthread_t squareThread[N];
		//Initialze paramaters array for all squares
		parameters squares[N];

		int x = 0;
		//loop through all starting square colums
		for (int countcolumn = 0; countcolumn <= 6; countcolumn += 3) {
			//loop through all starting square rows
			for (int countrow = 0; countrow <= 6; countrow += 3) {
				squares[x].column = countcolumn;
				squares[x].row = countrow;
				//Create thread using checkSquare function
				pthread_create(&squareThread[x], 0, checkSquare, (void *) &squares[x]);
				x++;
			}
		}
		//Join all threads
		pthread_join(rowThread, 0);
		pthread_join(columnThread, 0);

		for(int x = 0; x < N; x++){
			pthread_join(squareThread[x], 0);
		}

		return NULL;
}

//Initialize and define main function
int main() {
	//Open puzzle.txt in read only mode
	FILE *puzzle = fopen("puzzle.txt", "r");
	//read the sudoku in the file and assign it to global array
	readPuzzle(puzzle);
	//Open solution.txt in write only mode
	FILE *solution = fopen("solution.txt", "w");
	//Check if soduku is able to be solved
	if (sudokuSolver(0, 0)){
		printf("Solution found\n");

		//print the solution to solution.txt file
		for(int x = 0; x < N; x++){
			for(int y = 0; y < N; y++) {
				fprintf(solution, "%d ", sudoku[x][y]);

				if (y == 8) {
					fprintf(solution, "\n");
				}
			}
		}
		//Validate the solution using threads
		validate();
		//If isCorrect array has all ones then solution is correct
		if (isCorrect[0] == 1 && isCorrect[1] == 1 && isCorrect[2] == 1){
			printf("Sudoku is correct");
		//Else the solution is wrong
		} else {
			printf("Sudoku is incorrect");
		}
	} else {
		printf("No Solution\n");
	}

	return 0;
}