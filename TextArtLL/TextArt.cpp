/*
* This program allows display and editing of text art (also called ASCII art).
*/
#define NOMINMAX

#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;


/**
* @brief Main program allows display and editing of text art.
*
* @return 0 if program will run succesfully.
*/
int main()
{
	const int FILESIZE = FILENAMESIZE - 15;
	// char canvas[MAXROWS][MAXCOLS], 
	//char undo[MAXROWS][MAXCOLS];
	char old, newCh;
	char input;
	char fileLoad[FILENAMESIZE];
	int row, col;
	bool animate = false;
	bool flag = true;
	bool flagLoad = true;
	char animateStatus;
	//char menuMainTop[] = "<A>nimate: N / <U>ndo: 0 / Cl<I>p: 0\n";

	char menuMainBottom[] = "<E>dit / <M>ove / <R>eplace / <D>raw / <C>lear / <U>ndo / <L>oad / <S>ave / <Q>uit: ";
	//initCanvas(canvas);
	Node* current = newCanvas();
	Node* backUp = newCanvas(current);
	List undo;
	List redo;
	List clips;
	//addNode(undo, backUp);
	//initCanvas(undo);

	while (flag)
	{
		system("cls");
		displayCanvas(current->item);

		animateStatus = animate ? 'Y' : 'N';


		if (undo.count >= 0 && redo.count == 0 && clips.count < 2) //inital menu
		{
			printf("<A>nimate: %c / <U>ndo: %d / Cl<I>p: %d \n", animateStatus, undo.count, clips.count);

		}
		if (clips.count >= 2 && redo.count == 0) // with just play
		{
			printf("<A>nimate: %c / <U>ndo: %d / Cl<I>p: %d / <P>lay \n", animateStatus, undo.count, clips.count);
		}
		if (redo.count > 0 && clips.count < 2) // with just redo, if undo action was done
		{
			printf("<A>nimate: %c / <U>ndo: %d / Red<O>: %d / Cl<I>p: %d \n", animateStatus, undo.count, redo.count, clips.count);
		}
		if (redo.count > 0 && clips.count >= 2) // with the redo and play option 
		{
			printf("<A>nimate: %c / <U>ndo: %d / Red<O>: %d / Cl<I>p: %d / <P>lay \n", animateStatus, undo.count, redo.count, clips.count);
		}


		//printf("%s", menuMainTop);
		//printf("<A>nimate: %c / <U>ndo: %d %s%d / Cl<I>p: %d %s%c \n",animateStatus, undo.count,redoMenu, redo.count, clips.count, playMenu, playStatus);
		printf("%s", menuMainBottom);

		cin >> input;
		clearLine(MAXROWS + 2, MAXCOLS + BUFFERSIZE);
		clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
		input = toupper(input);


		switch (input)
		{
		case 'A':
			animate = !animate;
			break;
		case 'O': //redo
			restore(redo, undo, current);
			break;
		case 'I': //clips
			addNode(clips, newCanvas(current));
			break;
		case 'P':
			play(clips);
			break;
		case 'E':
			printf("Press <ESC> to exit.");
			addUndoState(undo, redo, current);
			editCanvas(current->item);
			break;
		case 'M':
			printf("Enter column units to move: ");
			cin >> col;
			printf("Enter row units to move: ");
			cin >> row;
			addUndoState(undo, redo, current);
			moveCanvas(current->item, row, col);

			break;
		case 'D':
			menuTwo(current, undo, redo, clips, animate);
			break;
		case 'C':
			addUndoState(undo, redo, current);
			initCanvas(current->item);
			break;
		case 'R':
			printf("Enter character to replace: ");
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.get(old);
			printf("Enter character to replace with: ");
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.get(newCh);
			addUndoState(undo, redo, current);
			replace(current->item, old, newCh);
			break;
		case 'U':
			if (undo.count > 0)
			{
				restore(undo, redo, current);
			}
			break;
		case 'L':
			printf("<C>anvas or <A>nimate ? ");
			cin >> input;
			input = toupper(input);
			

			if (input == 'C')
			{
				clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
				cin.ignore();
				cout << "Enter the filename (don't enter 'txt'): ";
				cin.getline(fileLoad, FILENAMESIZE);
				char preFix[FILENAMESIZE] = "SavedFiles\\";
				int length = 0;
				int lengthP = strlen(preFix);



				// Rewords the file name to be of use as a location in the SavedFiles folder
				snprintf(preFix + lengthP, FILENAMESIZE - lengthP, "%s.txt", fileLoad);
				addUndoState(undo, redo, current);
				flagLoad = loadCanvas(current->item, preFix);
				if (!flagLoad)
				{
					cerr << "ERROR: File cannot be read\n";
				}
			}
			else if (input == 'A')
			{
				clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
				cin.ignore();
				cout << "Enter the filename (don't enter 'txt'): ";
				cin.getline(fileLoad, FILENAMESIZE);
				char preFix[FILENAMESIZE] = "SavedFiles\\";
				int length = 0;
				int lengthP = strlen(preFix);


				// Rewords the file name to be of use as a location in the SavedFiles folder
				snprintf(preFix + lengthP, FILENAMESIZE - lengthP, "%s", fileLoad);

				flagLoad = loadClips(clips, preFix);

				if (!flagLoad)
				{
					cerr << "ERROR: File cannot be read\n";
				}
				else
				{
					cout << "Clips loaded!\n";
					system("pause");
				}	
				break;
			}
			break;
		case 'S':
			char filename[FILESIZE];
			cout << "<C>anvas or <A>nimation ?\n";
			cin >> input;
			input = toupper(input);

			if (input == 'C')
			{
				clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
				cout << "Enter the name of the file: (don't enter 'txt')";
				cin.clear();
				cin.ignore();
				cin.getline(filename, FILENAMESIZE);
				bool flagSave = saveCanvas(current->item, filename);

				if (!flagSave)
				{
					cout << "ERROR" << endl;
				}
				else
				{
					cout << "File Saved!" << endl;
				}
			}
			else if (input == 'A')
			{
				clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
				cout << "Enter the name of the file: (don't enter 'txt')";
				cin.clear();
				cin.ignore();
				cin.getline(filename, FILENAMESIZE);
				bool flagSave = saveClips(clips, filename);
				if (!flagSave)
				{
					cerr << "ERROR" << endl;
				}
				else
				{
					cout << "Clips Saved!" << endl;
					system("pause");
				}
				break;
			}
			break;
		case 'Q':
			deleteList(clips);
			deleteList(redo);
			deleteList(undo);
			delete current;
			flag = false;
		default:
			break;
		}

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
	gotoxy(0, 0);

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
bool saveCanvas(char canvas[][MAXCOLS], char filename[])
{
	char directory[FILENAMESIZE] = "SavedFiles";    //Creates the directory used to make sure we have the correct filepath

	char filePath[FILENAMESIZE];
	snprintf(filePath, FILENAMESIZE, "%s\\%s.txt", directory, filename);
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
		return true;
	}
	else
	{
		return false;
	}
}


/**
* Gets a filename from the user. If file can be opened for reading,
* loads the file's contents into canvas.
*
* @param[in] canvas Canvas to load
*/
bool loadCanvas(char canvas[][MAXCOLS], char filename[])
{
	char buffer[BUFFERSIZE];

	ifstream inFile;
	inFile.open(filename);

	// If the file can be found, go to the first position in the file and assigns all the characters
	if (inFile)
	{
		initCanvas(canvas);

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
							canvas[row][col] = c;
							c = inFile.get();
						}
					}
					else
					{
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
		return true;
	}
	else
	{

		return false;
	}
}