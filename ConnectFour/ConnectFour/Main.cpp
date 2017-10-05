#include <iostream>

using namespace std;

void play();
int matchDirection(char board[20][20], int xSize, int ySize, int xDirection, int yDirection, int currentX, int currentY, char currentChar, bool isWrapAroundMode);
bool checkForWin(char [20][20], int, int, int [20], int, int, bool, char);
void modifyRules(int &, int &, int &, bool &);
void printBoard(char [20][20], int, int);

int main() {

	bool isDone = false;

	while (!isDone) {
		cout << "Play? (y/n)" << endl;
		char input = 'n';
		cin >> input;

		switch (input) {
		case 'y':
			play();
			break;
		case 'n':
			isDone = true;
			break;
		default:
			cout << "Invalid Input" << endl;
			break;
		}
	}

	return 0;
}

void play() {
	// Initialize Variables
	const int xMin = 4, xMax = 20, yMin = 4, yMax = 20;
	int xSize = 7, ySize = 6, winChainLength = 4;
	bool isWrapAroundMode = false;
	char board[20][20];
	int lineCount[20] = {0};
	bool done = false;
	bool isXTurn = true;
	int roundTotal;
	int roundCurrent = 0;

	// Wipe Board (wanted to move this to a function but can't figure out how to return arrays
	for (int y = 0; y<20; y++)
	{
		for (int x = 0; x < 20; x++) {
			board[x][y] = '.';
		}
		cout << endl;
	}

	// Prompt user to modify rules
	modifyRules(xSize, ySize, winChainLength, isWrapAroundMode);
	roundTotal = xSize * ySize;

	while (!done) {
		int userInput;
		char currentChar;
		bool hasWinner = false;
		roundCurrent++;

		// Print out the board
		printBoard(board, xSize, ySize);

		// Output Turn
		if (isXTurn) {
			cout << "X's Turn: ";
			currentChar = 'X';
		} else {
			cout << "O's Turn: ";
			currentChar = 'O';
		}

		// Get user Input.  INPUT CHECK LATER-------
		bool inputIsValid = false;
		while (!inputIsValid) {
			cin >> userInput;
			if (userInput < 0 || userInput >= xSize || lineCount[userInput] >= ySize) {
				cout << "INVALID INPUT" << endl;
			} else {
				inputIsValid = true;
			}
		}
		

		// Place piece
		int currentPos = lineCount[userInput]++;
		board[userInput][currentPos] = currentChar;

		// Check for win
		hasWinner = checkForWin(board, xSize, ySize, lineCount, userInput, winChainLength, isWrapAroundMode, currentChar);
		
		if (hasWinner) {
			// Game is done
			printBoard(board, xSize, ySize);
			done = true;
			if (isXTurn) {
				cout << "X WINS" << endl;
			}
			else {
				cout << "O WINS" << endl;
			}
		} else {
			if (roundCurrent >= roundTotal) {
				// End if board full
				printBoard(board, xSize, ySize);
				cout << "DRAW" << endl;
				done = true;
			} else {
				// Flip turn if not done
				isXTurn = !isXTurn;
			}
		}
	}
}

bool checkForWin(char board[20][20], int xSize, int ySize, int lineCount[20], int userInput, int winChainLength, bool isWrapAroundMode, char currentChar) {
	int currentX = userInput;
	int currentY = lineCount[userInput] - 1;
	winChainLength--;

	// Check left right [-1][0] && [1][0]
	if (matchDirection(board, xSize, ySize, -1, 0, currentX, currentY, currentChar, isWrapAroundMode) + matchDirection(board, xSize, ySize, 1, 0, currentX, currentY, currentChar, isWrapAroundMode) >= winChainLength) {
		return true;
	}

	// Check up down [0][-1] && [0][1]
	if (matchDirection(board, xSize, ySize, 0, -1, currentX, currentY, currentChar, isWrapAroundMode) + matchDirection(board, xSize, ySize, 0, 1, currentX, currentY, currentChar, isWrapAroundMode) >= winChainLength) {
		return true;
	}

	// Check upward diagonal [-1][-1] && [1][1]
	if (matchDirection(board, xSize, ySize, -1, -1, currentX, currentY, currentChar, isWrapAroundMode) + matchDirection(board, xSize, ySize, 1, 1, currentX, currentY, currentChar, isWrapAroundMode) >= winChainLength) {
		return true;
	}

	// Check downward diagonal [-1][1] && [1][-1]
	if (matchDirection(board, xSize, ySize, -1, 1, currentX, currentY, currentChar, isWrapAroundMode) + matchDirection(board, xSize, ySize, 1, -1, currentX, currentY, currentChar, isWrapAroundMode) >= winChainLength) {
		return true;
	}

	return false;
}

int matchDirection(char board[20][20], int xSize, int ySize, int xDirection, int yDirection, int currentX, int currentY, char currentChar, bool isWrapAroundMode) {
	int matchCount = 0;
	int newX = currentX + xDirection;
	int newY = currentY + yDirection;
	
	// Boundary Checking
	if (newX >= xSize) {
		if (!isWrapAroundMode) {
			return matchCount;
		} else {
			newX = 0;
		}
	}

	if (newX < 0) {
		if (!isWrapAroundMode) {
			return matchCount;
		} else {
			newX = xSize - 1;
		}
	}
	
	if (newY < 0 || newY >= ySize) {
		return matchCount;
	}

	// Recurse
	if (board[newX][newY] == currentChar) {
		matchCount++;
		matchCount += matchDirection(board, xSize, ySize, xDirection, yDirection, newX, newY, currentChar, isWrapAroundMode);
	}

	return matchCount;
}

void modifyRules(int &x, int &y, int &chainLength, bool &isWrapAroundMode) {
	bool isDone = false;

	// INPUT CHECK THIS LATER-------
	cout << "Modify Rules? (y/n)" << endl;
	isDone = false;
	while (!isDone) {
		char input;
		cin >> input;

		switch (input) {
		case 'y':
			isDone = true;
			break;
		case 'n':
			return;
			break;
		default:
			cout << "INVALID INPUT" << endl;
			break;
		}
	}

	cout << "# Columns (4-20)" << endl;
	isDone = false;
	while (!isDone) {
		int input;
		cin >> input;

		if (input < 4 || input > 20) {
			cout << "INVALID INPUT" << endl;
		} else {
			x = input;
			isDone = true;
		}
	}

	cout << "# Rows (4-20)" << endl;
	isDone = false;
	while (!isDone) {
		int input;
		cin >> input;

		if (input < 4 || input > 20) {
			cout << "INVALID INPUT" << endl;
		}
		else {
			y = input;
			isDone = true;
		}
	}

	cout << "# Matches to Win (3-" << x << ")" << endl;
	isDone = false;
	while (!isDone) {
		int input;
		cin >> input;

		if (input < 3 || input > x) {
			cout << "INVALID INPUT" << endl;
		}
		else {
			chainLength = input;
			isDone = true;
		}
	}
	
	// Wrap around mode
	isDone = false;
	while (!isDone) {
		cout << "Wrap Around Mode? (y/n)" << endl;

		char input;
		cin >> input;

		switch (input) {
		case 'y':
			isWrapAroundMode = true;
			isDone = true;
			break;
		case 'n':
			isWrapAroundMode = false;
			isDone = true;
			break;
		default:
			cout << "Invalid Input" << endl;
			break;
		}
	}
}

void printBoard(char board[20][20], int xSize, int ySize) {
	for (int y=-1; y<ySize; y++)
	{
		for (int x = 0; x < xSize; x++) {
			if (y<0) {
				cout << x << " ";
				if (x < 10) {
					cout << " ";
				}
			} else {
				cout << board[x][ySize-y-1] << "  ";
			}
		}
		cout << endl;
	}
}