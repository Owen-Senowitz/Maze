// Author: Owen Senowitz
// Tries to solve a maze using a recursive solution
// Shows path through the maze if one exists

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;

const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 81;

// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
const char WALL = (char)219;
const char WALLINFILE = '0';
const char START = 'S';
const char DEST = 'D';
const char EMPTY = ' ';
const char PATH = '*';
const char DEAD = 'X';

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and store the maze
* structure into the maze array, based on the contents of the file.
* A WALLINFILE character in the file represents a maze wall, while
* an EMPTY character represents a free pathway. The file should also
* contain a single START character and a single DEST character.
* The function stops reading and exits the program if > MAXCOLS-1
* characters are found on one row, or if > MAXROWS rows are
* found in the file.  If the function is successful, then
* startX and startY will be set to the row and column number of
* where the START character is located in the maze.
*/
void getMaze(char maze[][MAXCOLS], int& startX, int& startY);

/*
* display is used to draw the maze on the screen.
*/
void display(const char maze[][MAXCOLS]);

// TODO: Declare the remaining functions you need    

// EXTRA CREDIT: counts the number of PATH and DEAD to recall amount of recursive calls made.
int countCalls(char maze[][MAXCOLS]);

// removes the "X" in the maze
void removeDEAD(char maze[][MAXCOLS]);

// searches the maze for the correct way
bool searchMaze(char maze[][MAXCOLS], const int& startX, const int& startY);

/*
* intializes the world for the maze by putting
* 80 spaces in each row and makes it a string
*/
void initWorld(char maze[][MAXCOLS]);

// main function
int main()
{
	char maze[MAXROWS][MAXCOLS];
	int startX, startY;
	bool searchSolution;
	int counter = 0;

	// TODO: Write the main function
	initWorld(maze);
	getMaze(maze, startX, startY);
	display(maze);

	cout << "Press ENTER to solve maze ";
	cin.get();

	searchSolution = searchMaze(maze, startX, startY);

	counter = countCalls(maze);

	// clean maze
	removeDEAD(maze);

	// store s back in maze
	maze[startX][startY] = START;

	if (searchSolution)
	{
		display(maze);
		cout << "Solution Found! " << counter << " recursive calls" << endl;
	}

	else
	{
		display(maze);
		cout << "No solution to this maze. " << counter << " recursive calls" << endl;
	}
	return 0;
}

// TODO: Write the code for the remaining functions that you need

// this function will display the maze
void display(const char maze[][MAXCOLS])
{
	// system("cls") clears the screen; only works on Windows machines
	// You will probably need to comment this out if working on
	// a non-Windows machine
	system("cls");

	// TODO: Write the display function

	// counts through all the rows
	for (int i = 0; i < MAXROWS; i++)
	{
		// counts through all the columns
		cout << maze[i] << endl;
	}

	cout << endl;

}

void getMaze(char maze[][MAXCOLS], int& startX, int& startY)
{
	// TODO: Write the getMaze function

	ifstream inFile;
	char fileName[FILENAMESIZE];
	char tempArray[MAXCOLS];
	int x = 0;

	// this code will ask for and open the file
	cout << "Enter the name of a TXT file containing the maze (don't enter 'txt'): " << endl;
	cin.getline(fileName, FILENAMESIZE - 4);

	size_t filled = strlen(fileName);

	strncat(fileName, ".txt", FILENAMESIZE - filled);

	inFile.open(fileName);
	system("cls");

	// if file is open then we run a loop to read the file
	if (!inFile.is_open())
	{
		// this code will run if the imputed file name can not be opened
		cerr << "\nERROR: Input file cannot be opened.\n";
		inFile.close();
		exit(1);
	}

	while (x < MAXROWS && inFile.getline(tempArray, MAXCOLS))
	{
		size_t length = strlen(tempArray);

		for (int i = 0; i < MAXCOLS; i++)
		{
			if (tempArray[i] == WALLINFILE)
			{
				maze[x][i] = WALL;
			}

			else
			{
				maze[x][i] = tempArray[i];

				if (tempArray[i] == START)
				{
					startX = x;
					startY = i;
				}
			}
		}
		x++;
	}

	// check for errors in reading file
	if (x > MAXCOLS - 1)
	{
		cerr << "\nERROR: Line " << x << " in input file contains more than " << MAXCOLS - 1 << " chars\n";
		inFile.close();
		exit(1);
	}
	
	if (x > MAXROWS)
	{
		cerr << "\nERROR: There are more than " << MAXROWS << " lines in the file\n";
		inFile.close();
		exit(1);
	}

	inFile.close();
}

void initWorld(char maze[][MAXCOLS])
{
	// goes through an array and fills with empty and then becomes a string
	char emptyArray[MAXCOLS];

	for (int c = 0; c < MAXCOLS; c++)
	{
		emptyArray[c] = EMPTY;
	}

	emptyArray[MAXCOLS - 1] = '\0';

	for (int r = 0; r < MAXROWS; r++)
	{
		snprintf(maze[r], MAXCOLS, emptyArray);
	}
}

// TODO: Declare the remaining functions you need

bool searchMaze(char maze[][MAXCOLS], const int& startX, const int& startY)
{
	// Base case: If indices startX and startY are at the DEST search is over.
	if (maze[startX][startY] == DEST)
	{
		return true;
	}

	else if (maze[startX][startY] == EMPTY || maze[startX][startY] == START)
	{
		// assumption that start is on the path by default
		maze[startX][startY] = PATH;

		if (searchMaze(maze, startX - 1, startY))
		{
			return true;
		}

		else if (searchMaze(maze, startX + 1, startY))
		{
			return true;
		}

		else if (searchMaze(maze, startX, startY - 1))
		{
			return true;
		}

		else if (searchMaze(maze, startX, startY + 1))
		{
			return true;
		}

		else
		{
			maze[startX][startY] = DEAD;
			return false;
		}
	}

	else
	{
		return false;
	}
}

// this function takes all of the X's that lead to dead ends and removes them from the display.
void removeDEAD(char maze[][MAXCOLS])
{
	for (int col = 0; col < MAXCOLS; col++)
	{
		for (int row = 0; row < MAXROWS; row++)
		{
			if (maze[row][col] == DEAD)
			{
				maze[row][col] = EMPTY;
			}
		}
	}
}

/*
* EXTRA CREDIT: This function attempts to print out the count of the number of recursive function calls that the program makes
*/
int countCalls(char maze[][MAXCOLS])
{
	int count = 0;
	int countDEAD = 0;

	for (int cols = 0; cols < MAXCOLS; cols++)
	{
		for (int rows = 0; rows < MAXROWS; rows++)
		{
			if (maze[rows][cols] == PATH)
			{
				count++;
			}
			else if (maze[rows][cols] == DEAD)
			{
				countDEAD++;
			}
		}
	}
	return count + (2 * countDEAD);
}
