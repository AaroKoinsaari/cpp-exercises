/*
* This program allows display and editing of text art (also called ASCII art).
*/
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
using namespace std;

const int BUFFERSIZE = 20;
const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 80;

// ASCII codes for special keys; for editing
const char ESC = 27;
const char LEFTARROW = 75;
const char UPARROW = 72;
const char RIGHTARROW = 77;
const char DOWNARROW = 80;
const char SPECIAL = (char)224;

/*
* Gets a filename from the user. If file can be opened for reading,
* this function loads the file's contents into canvas.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed and
* canvas is left unchanged.
*/
void loadCanvas(char canvas[][MAXCOLS]);

/*
* Gets a filename from the user. If file can be opened for writing,
* this function writes the canvas contents into the file.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed.
*/
void saveCanvas(char canvas[][MAXCOLS]);

/*
* Initializes canvas to contain all spaces.
*/
void initCanvas(char canvas[][MAXCOLS]);

/*
* Displays canvas contents on the screen, with a border
* around the right and bottom edges.
*/
void displayCanvas(char canvas[][MAXCOLS]);

/*
* Allows user to edit the canvas by moving the cursor around and
* entering characters. Editing continues until the ESC key is pressed.
*/
void editCanvas(char canvas[][MAXCOLS]);

/*
* Copies contents of the "from" canvas into the "to" canvas.
*/
void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS]);

/*
* Replaces all instances of a character in the canvas.
* oldCh is the character to be replaced.
* newCh character is the character to replace with.
*/
void replace(char canvas[][MAXCOLS], char oldCh, char newCh);

/*
* Shifts contents of the canvas by a specified number of rows and columns.
* rowValue is the number of rows by which to shift
*    positive numbers shift downward; negative numbers shift upward
* colValue is the number of rows by which to shift
*    positive numbers shift right; negative numbers shift left
*/
void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue);

/*
* Clears a line on the output screen, then resets the cursor back to the
* beginning of this line.
* lineNum is the line number on the output screen to clear
* numOfChars is the number of characters to clear on this line
*/
void clearLine(int lineNum, int numOfChars);

/*
* Moves the cursor in the output window to a specified row and column.
* The next output produced by the program will begin at this position.
*/
void gotoxy(short row, short col);

// TODO: Fix the problem with quitting the program
// TODO: Fix the '|' sign going weird places (maybe problem with displayCanvas
// or initCanvas?)
// TODO: some kind of error handling to editCanvas function if needed(?)


/**
* @brief Main program allows display and editing of text art.
* 
* @return 0 if program will run succesfully.
*/
int main()
{
	char canvas[MAXROWS][MAXCOLS], undo[MAXROWS][MAXCOLS];
	char old, newCh;
	char input;
	int row, col;
	initCanvas(canvas);
	initCanvas(undo);
	while (1)
	{
		displayCanvas(canvas);
		printf("<E>dit / <M>ove / <R>eplace / <U>ndo / <L>oad / <S>ave / <Q>uit: ");

		cin >> input;
		clearLine(MAXROWS + 1, MAXCOLS);

		switch (input)
		{
			case 'E':
			case 'e':
				printf("Press <ESC> to exit.");
				copyCanvas(undo, canvas);
				editCanvas(canvas);
				break;
			case 'M':
			case 'm':
				printf("Enter column units to move: ");
				cin >> col;
				printf("Enter row units to move: ");
				cin >> row;
				copyCanvas(undo, canvas);
				moveCanvas(canvas, row, col);
				break;
			case 'R':
			case 'r':
				printf("Enter character to replace: ");
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin.get(old);
				printf("Enter character to replace with: ");
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin.get(newCh);
				copyCanvas(undo, canvas);
				replace(canvas, old, newCh);
				break;
			case 'U':
			case 'u':
				copyCanvas(canvas, undo);
				break;
			case 'L':
			case 'l':
				cin.ignore();
				copyCanvas(undo, canvas);
				loadCanvas(canvas);
				break;
			case 'S':
			case 's':
				saveCanvas(canvas);
				break;
			case 'Q':
			case 'q':
				return 0;
			default:
				break;
		}
		clearLine(MAXROWS + 2, MAXCOLS);
	}
}


/**
* Moves the cursor in the output window to a specified row and column.
* The next output produced by the program will begin at this position.
* 
* @param row Row to move the cursor to
* @param col Column to move the cursor to
*/
void gotoxy(short row, short col)
{
	COORD pos = { col, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


/**
* Clears a line on the output screen, then resets the cursor back to the
* beginning of this line.
* 
* @param lineNum Line number on the output screen to clear
* @param numOfChars Number of characters to clear on this line
*/
void clearLine(int lineNum, int numOfChars)
{
	// Move cursor to the beginning of the specified line on the console
	gotoxy(lineNum, 0);

	// Write a specified number of spaces to overwrite characters
	for (int x = 0; x < numOfChars; x++)
		cout << " ";

	// Move cursor back to the beginning of the line
	gotoxy(lineNum, 0);
}


/**
* Replaces all instances of a character in the canvas.
* 
* @param canvas[in] Array to update
* @param oldCh The character to be replaced
* @param newCh The character to replace with
*/
void replace(char canvas[][MAXCOLS], char oldCh, char newCh)
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			if (canvas[i][j] == oldCh)
			{
				canvas[i][j] = newCh;
			}
		}
	}
}


/**
* Allows user to edit the canvas by moving the cursor
* around and entering characters until ESC is pressed.
* 
* @param[in] canvas Array that needs to be updated
*/
void editCanvas(char canvas[][MAXCOLS])
{
	char input = 0;
	int row = 0, col = 0;

	// Move cursor to row,col and then get
	// a single character from the keyboard
	gotoxy(row, col);

	// Get input from user until ESC is pressed
	while (input != ESC) {
		input = _getch();

		// Arrow keys
		if (input == SPECIAL) {

			input = _getch();

			switch (input) {
			case LEFTARROW:
				if (col > 0) col--;
				break;
			case RIGHTARROW:
				if (col < MAXCOLS - 1) col++;
				break;
			case UPARROW:
				if (row > 0) row--;
				break;
			case DOWNARROW:
				if (row < MAXROWS - 1) row++;
				break;
			default:
				break;
			}
		}

		// Ignore the function keys
		else if (input == '\0') {
			input = _getch();
		}

		// Printable ASCII characters
		else if (input >= 32 && input <= 126) {
			canvas[row][col] = input;
			cout << input;
		}

		// Ignore other characters
		else {
			input = _getch();
		}

		gotoxy(row, col); // Moves cursor to the new position
	}
}


/**
* Shifts contents of the canvas by a specified number of rows and columns.
* 
* @param[in] canvas Canvas to update
* @param rowValue Number of rows by which to shift
* @param colValue Number of columns by which to shift
*/
void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue)
{
	char temp[MAXROWS][MAXCOLS];
	initCanvas(temp);
	for (int x = 0; x < MAXROWS; x++)
	{
		if (x + rowValue < MAXROWS && x + rowValue >= 0)
		{
			for (int y = 0; y < MAXCOLS; y++)
			{
				if (y + colValue < MAXCOLS && y + colValue >= 0)
				{
					temp[x + rowValue][y + colValue] = canvas[x][y];
					canvas[x][y] = ' ';
				}
			}
		}
	}
	copyCanvas(canvas, temp);
}


/**
* Initializes canvas to contain all spaces.
* 
* @param canvas[in] Canvas to initialize
*/
void initCanvas(char canvas[][MAXCOLS])
{
	// Initializes spaces in all spots in array
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			canvas[i][j] = ' ';
		}
	}
}


/**
* Displays canvas contents on the screen, with a border
* around the right and bottom edges.
*
* @param[in] Canvas to display
*/
void displayCanvas(char canvas[][MAXCOLS])
{
	// Clear the screen
	system("cls");

	for (int rowCount = 0; rowCount < MAXROWS; rowCount++)
	{
		for (int colCount = 0; colCount < MAXCOLS; colCount++)
		{
			cout << canvas[rowCount][colCount];        // Goes through and prints the array without any borders added 
		}
		cout << "|" << endl;    // Prints the right side of the border since when we reach this point colCount == MAXCOLS - 1

	}

	for (int bottomBorder = 0; bottomBorder < MAXCOLS; bottomBorder++)
	{
		cout << "-"; // Used to print the bottom border of the canvas.
	}
	cout << endl;
}


/**
* Copies contents of the "from" canvas into the "to" canvas.
*
* @param[in] to Canvas to copy
* @param[in] from Canvas to copy from
*/
void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS])
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			to[i][j] = from[i][j];
		}
	}
}


/**
* Gets a filename from the user. If file can be opened for writing,
* writes the canvas contents into the file.
* 
* @param canvas[in] Canvas to save
*/
void saveCanvas(char canvas[][MAXCOLS])
{
	char directory[FILENAMESIZE] = "SavedFiles";    //Creates the directory used to make sure we have the correct filepath

	int preFix = 5;
	int length = strlen(directory) + preFix;
	const int FILESIZE = 240;
	char fileName[FILESIZE];    //Array to hold the name of the file

	//Asks user to name the new file they wish to save
	cout << "Enter the name of the file: ";
	cin.clear();
	cin.ignore();
	cin.getline(fileName, FILESIZE);
	/*if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}*/

	//Creates a filpath that we can use to put our filename with the correct directories 
	char filePath[FILENAMESIZE];
	snprintf(filePath, FILENAMESIZE, "%s\\%s.txt", directory, fileName);

	//Creates the filepath to be written into
	ofstream myFile(filePath);
	//checks if the file is open, writes the contents of canvas into it
	if (myFile)
	{
		for (int x = 0; x < MAXROWS; x++)
		{
			for (int y = 0; y < MAXCOLS; y++)
			{
				myFile << canvas[x][y];
			}
			myFile << endl;
		}
		//closes the file and lets the user know that the file was saved.
		myFile.close();
		cout << "File saved!\n";
		system("pause");

	}
	//Outputs an error message if the file cannot be opened.
	else
	{
		cerr << "ERROR: file could not be saved\n";
		system("pause");
	}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


/**
* Gets a filename from the user. If file can be opened for reading,
* loads the file's contents into canvas.
* 
* @param[in] canvas Canvas to load
*/
void loadCanvas(char canvas[][MAXCOLS])
{
 	char tempCanvas[MAXROWS][MAXCOLS];
	char buffer[BUFFERSIZE];
	initCanvas(tempCanvas);
	initCanvas(canvas);
	char fileLoad[FILENAMESIZE];
	char preFix[FILENAMESIZE] = "SavedFiles\\";
	int length = 0;
	int lengthP = strlen(preFix);

	cout << "Enter the filename (don't enter 'txt'): ";
	cin.getline(fileLoad, FILENAMESIZE - length);
	
	// Rewords the file name to be of use as a location in the SavedFiles folder

	snprintf(preFix + lengthP, FILENAMESIZE - lengthP, "%s.txt", fileLoad);

	ifstream inFile;
	inFile.open(preFix);

	// If the file can be found, go to the first position in the file and assigns all the characters
	if (inFile)
	{
		system("cls");
		gotoxy(0, 0);

		int c = inFile.get();
		for (int row = 0; row <= MAXROWS + BUFFERSIZE && !inFile.eof();)
		{
			for (int col = 0; col <= MAXCOLS + BUFFERSIZE && !inFile.eof(); col++)
			{
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				else
				{
					if (row < MAXROWS && col < MAXCOLS)
					{
						if (c == '\n')
						{
							row++;
							col = -1;
							c = inFile.get();
						}
						else
						{
							tempCanvas[row][col] = c;
							c = inFile.get();
						}
					}
					else
					{
						/*if(c != '\n')
						{
							buffer[0] = c;
							c = inFile.get();
						}
						if(c == '\n')
						{
							row++;
							col = -1;
							c = inFile.get();
						}*/
						while (c != '\n')
						{
							buffer[0] = c;
							c = inFile.get();
						}
						row++;
						col = -1;
						c = inFile.get();
					}
				}
			}
		}
		inFile.close();
		copyCanvas(canvas, tempCanvas);
	}
	else
	{
		cerr << "ERROR: File cannot be read.\n";
		system("pause");
	}
}