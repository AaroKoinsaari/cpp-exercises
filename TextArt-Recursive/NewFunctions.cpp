#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Definitions.h"
using namespace std;


Point::Point(DrawPoint p)
{
	row = (int)round(p.row);
	col = (int)round(p.col);
}

// https://math.stackexchange.com/questions/39390/determining-end-coordinates-of-line-with-the-specified-length-and-angle
DrawPoint findEndPoint(DrawPoint start, int len, int angle)
{
	DrawPoint end;
	end.col = start.col + len * cos(degree2radian(angle));
	end.row = start.row + len * sin(degree2radian(angle));
	return end;
}


// Use this to draw characters into the canvas, with the option of performing animation
void drawHelper(char canvas[][MAXCOLS], Point p, char ch, bool animate)
{
	// Pause time between steps (in milliseconds)
	const int TIME = 50;

	// Make sure point is within bounds
	if (p.row >= 0 && p.row < MAXROWS && p.col >= 0 && p.col < MAXCOLS)
	{
		// Draw character into the canvas
		canvas[p.row][p.col] = ch;

		// If animation is enabled, draw to screen at same time
		if (animate)
		{
			gotoxy(p.row, p.col);
			printf("%c", ch);
			Sleep(TIME);
		}
	}
}


// Fills gaps in a row caused by mismatch between match calculations and screen coordinates
// (i.e. the resolution of our 'canvas' isn't very good)
void drawLineFillRow(char canvas[][MAXCOLS], int col, int startRow, int endRow, char ch, bool animate)
{
	// determine if we're counting up or down
	if (startRow <= endRow)
		for (int r = startRow; r <= endRow; r++)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
	else
		for (int r = startRow; r >= endRow; r--)
		{
			Point point(r, col);
			drawHelper(canvas, point, ch, animate);
		}
}


// Draw a single line from start point to end point
void drawLine(char canvas[][MAXCOLS], DrawPoint start, DrawPoint end, bool animate)
{
	char ch;

	Point scrStart(start);
	Point scrEnd(end);

	// vertical line
	if (scrStart.col == scrEnd.col)
	{
		ch = '|';

		drawLineFillRow(canvas, scrStart.col, scrStart.row, scrEnd.row, ch, animate);
	}
	// non-vertical line
	else
	{
		int row = -1, prevRow;

		// determine the slope of the line
		double slope = (start.row - end.row) / (start.col - end.col);

		// choose appropriate characters based on 'steepness' and direction of slope
		if (slope > 1.8)  ch = '|';
		else if (slope > 0.08)  ch = '`';
		else if (slope > -0.08)  ch = '-';
		else if (slope > -1.8) ch = '\'';
		else ch = '|';

		// determine if columns are counting up or down
		if (scrStart.col <= scrEnd.col)
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col <= scrEnd.col; col++)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
		else
		{
			// for each column from start to end, calculate row values
			for (int col = scrStart.col; col >= scrEnd.col; col--)
			{
				prevRow = row;
				row = (int)round(slope * (col - start.col) + start.row);

				// draw from previous row to current row (to fill in row gaps)
				if (prevRow > -1)
				{
					drawLineFillRow(canvas, col, prevRow, row, ch, animate);
				}
			}
		}
	}
}


// Draws a single box around a center point
void drawBox(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	int sizeHalf = height / 2;
	int ratio = (int)round(MAXCOLS / (double)MAXROWS * sizeHalf);

	// Calculate where the four corners of the box should be
	DrawPoint points[4];
	points[0] = DrawPoint(center.row - sizeHalf, center.col - ratio);
	points[1] = DrawPoint(center.row - sizeHalf, center.col + ratio);
	points[2] = DrawPoint(center.row + sizeHalf, center.col + ratio);
	points[3] = DrawPoint(center.row + sizeHalf, center.col - ratio);

	// Draw the four lines of the box
	for (int x = 0; x < 3; x++)
	{
		drawLine(canvas, points[x], points[x + 1], animate);
	}
	drawLine(canvas, points[3], points[0], animate);

	// Replace the corners with a better looking character
	for (int x = 0; x < 4; x++)
	{
		drawHelper(canvas, points[x], '+', animate);
	}
}


// Menu for the drawing tools
void menuTwo(char canvas[][MAXCOLS], char backupCanvas[][MAXCOLS], bool& animate)
{
	// TODO: Write the code for the function
	char menuSelection;
	int height = 0;
	int heightNestedBox = 0;
	int startAngle = 270;
	int branchAngle = 0;
	int row = 0;
	int col = 0;
	char oldCh = '0';
	char newCh = '0';
	char pos;
	Point Center;
	Point Start, end;
	int heightBox = 0;

	char menu[] = "<A>nimate: N / <F>ill / <L>ine / <B>ox / <N>ested Boxes / <T>ree / <M>ain Menu: ";

	while(1)
	{
		displayCanvas(canvas);
		if (animate == true)
		{
			menu[11] = 'Y';
		}
		else
		{
			menu[11] = 'N';
		}
		printf("%s", menu);
		cin >> menuSelection;
		clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);

		switch (menuSelection)
		{
		case 'A':
		case 'a':
			animate = !animate;			
			break;
		case 'F':
		case 'f':
			copyCanvas(backupCanvas, canvas);
			cout << "Enter character to fill with from current location / <ESC> to cancel";
			menuSelection = getPoint(Start);
			if (menuSelection != ESC)
			{
				fillRecursive(canvas, Start.row, Start.col, canvas[Start.row][Start.col], menuSelection, animate);
			}
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			break;
		case 'L':
		case 'l':
			copyCanvas(backupCanvas, canvas);
			printf("Type any letter to choose start point / <ESC> to cancel");
			pos = getPoint(Start);
			if (pos != ESC)
			{
				gotoxy(MAXROWS + 1, 0); // brings cursor back to print next statement
				clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
				printf("Type any letter to choose end point / <ESC> to cancel");
				pos = getPoint(end);
				if (pos != ESC)
				{
					drawLine(canvas, Start, end, animate);
				}
			}
			break;
		case 'B':
		case 'b':
			copyCanvas(backupCanvas, canvas);
			cout << "Enter size: ";
			cin >> heightBox;
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			printf("Type any letter to choose box center, or <C> for screen center / <ESC> to cancel");
			pos = getPoint(Center);
			if (pos != ESC)
			{
				if (pos == 'C' || pos == 'c')
				{
					Center.row = MAXROWS / 2;
					Center.col = MAXCOLS / 2;
				}
				drawBox(canvas, Center, heightBox, animate);
			}
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			break;
		case 'N':
		case 'n':
			copyCanvas(backupCanvas, canvas);
			cout << "Enter size of largest box: ";
			cin >> heightNestedBox;
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			cout << "Type any letter to choose box center, or <C> for center / <ESC>";
			pos = getPoint(Center);
			if (pos != ESC)
			{
				if (pos == 'C' || pos == 'c')
				{
					Center.row = MAXROWS / 2;
					Center.col = MAXCOLS / 2;
				}
				drawBoxesRecursive(canvas, Center, heightNestedBox, animate);
			}
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			break;
		case 'T':
		case 't':
			copyCanvas(backupCanvas, canvas);
			cout << "Enter approximate tree height: ";
			cin >> height;
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			cout << "Enter branch angle: ";
			cin >> branchAngle;
			clearLine(MAXROWS + 1, MAXCOLS + BUFFERSIZE);
			printf("Type any letter to choose a start point, or <C> for bottom center / <ESC> to cancel");
			pos = getPoint(Center);
			if (pos != ESC)
			{
				if (pos == 'C' || pos == 'c')
				{
					Center.row = MAXROWS - 1;
					Center.col = MAXCOLS / 2;
				}
				treeRecursive(canvas, Center, height, startAngle, branchAngle, animate);
			}
			break;
		case 'M':
		case 'm':
			return;
		}
	}
}


// Get a single point from screen, with character entered at that point
char getPoint(Point& pt)
{
	char input;

	int row = 0;
	int col = 0;
	//Move cursor to row,col and then get a signle character from the keybaord, so like initialize it 
	gotoxy(row, col);
	input = _getch();

	//from edit function
	while (1) 
	{

		// Arrow keys
		if (input == SPECIAL) 
		{

			input = _getch();

			switch (input) 
			{
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
		else if (input == '\0') 
		{
			input = _getch();
		}

		// Printable ASCII characters
		else if (input >= 32 && input <= 126) 
		{
			pt.row = row;
			pt.col = col;
			cout << input;
			return input;
		}

		else if (input == ESC)
		{
			return ESC;
		}

		gotoxy(row, col); // Moves cursor to the new position
		input = _getch(); //get user unput
	}
}


// Recursively fill a section of the screen
void fillRecursive(char canvas[][MAXCOLS], int row, int col, char oldCh, char newCh, bool animate)
{
	Point p;
	p.row = row;
	p.col = col;

	// base case: checks if it is outside of the array
	if (row > MAXROWS || row < 0 || col > MAXCOLS || col < 0)
		return;

	// base case: the character at current position is not being replaced
	if (canvas[row][col] != oldCh)
		return;

	// recursive cases
	drawHelper(canvas, p, newCh, animate);
	fillRecursive(canvas, row, col - 1, oldCh, newCh, animate);
	fillRecursive(canvas, row + 1, col, oldCh, newCh, animate);
	fillRecursive(canvas, row - 1, col, oldCh, newCh, animate);
	fillRecursive(canvas, row, col + 1, oldCh, newCh, animate);
}


// Recursively draw a tree
void treeRecursive(char canvas[][MAXCOLS], DrawPoint start, int height, int startAngle, int branchAngle, bool animate)
{
	//base case, check to make sure the tree is the minimum height
	if (height < 3)
	{
		return;
	}
	//Correctly initializes trunkHeight
	int trunkHeight = height / 3;

	//Ensures that the the angle will never be allowed to have a value over 359
	startAngle = startAngle % 360;

	//finds the endpoint of the trunk
	DrawPoint endTrunk = findEndPoint(start, trunkHeight, startAngle);

	//draws the trunk
	drawLine(canvas, start, endTrunk, animate);

	//Recursive steps to draw the branches
	treeRecursive(canvas, endTrunk, height - 2, startAngle - branchAngle, branchAngle, animate);
	treeRecursive(canvas, endTrunk, height - 2, startAngle + branchAngle, branchAngle, animate);

}


// Recursively draw nested boxes
void drawBoxesRecursive(char canvas[][MAXCOLS], Point center, int height, bool animate)
{
	//base case
	if (height <= 1)
	{
		return;
	}
	// Draw the box with the given height
	drawBox(canvas, center, height, animate);
	drawBoxesRecursive(canvas, center, height - 2, animate);
}