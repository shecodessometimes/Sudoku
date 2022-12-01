///////////////////////////////////////////////////////////////////////////////
// Project#4
// Sudoku: Part B
// Written by:
// James Napier:                                      napier.j@northeastern.edu
// Julia Rasmussen:                                rasmussen.j@northeastern.edu
// Samuel Sheehan:                                   sheehan.s@northeastern.edu
//
// Main program file for homework 4b. Includes functions for initializing the
// conflicts, adding and removing cells, and printing out the sudoku board.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include "board.h"
#include <list>
#include <math.h>
#include <fstream>

#pragma region Board

/*
* Initializes the board object.
*/
board::board(int sqSize)
    : value(BoardSize + 1, BoardSize + 1)
{
    row_conflicts = matrix<int>(BoardSize + 1, BoardSize + 1);
    col_conflicts = matrix<int>(BoardSize + 1, BoardSize + 1);
    square_conflicts = matrix<int>(BoardSize + 1, BoardSize + 1);
    clear();
}

/*
* Mark all possible values as legal for each board entry.
*/
void board::clear()
{
    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            value[i][j] = Blank;
            row_conflicts[i][j] = Blank;
            col_conflicts[i][j] = Blank;
            square_conflicts[i][j] = Blank;
        }
}

/*
* Read a Sudoku board from the input file fin.
*/
void board::initialize(ifstream& fin)
{
    char ch;

    clear();

    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            fin >> ch;

            // If the read char is not Blank
            if (ch != '.')
                setCell(i, j, ch - '0');   // Convert char to int
        }

}

/*
* Set the value at cell at index (r, c) to n.
*/
void board::setCell(int r, int c, int n)
{
    value[r][c] = n;
    updateConflicts(r, c, n, true);
}

/*
* Return the square number of cell (i,j) (counting from left to right,
* top to bottom).  Note that i and j each go from 1 to BoardSize.
*/
int squareNumber(int i, int j)
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that (i, j) is in.  

    return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

/*
* Overloaded output operator for vector class. Returns ostream with
* the printed out vector v, using the initial ostream ostr.
*/
ostream& operator<<(ostream& ostr, vector<int>& v)
{
    for (int i = 0; i < v.size(); i++)
        ostr << v[i] << " ";
    cout << endl;
    return ostr;
}

/*
* Returns the value stored in cell (i, j).  Throws an exception
* if bad values are passed.
*/
ValueType board::getCell(int i, int j)
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

/*
* Returns true if cell (i, j) is blank, and false otherwise.
*/
bool board::isBlank(int i, int j)
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad value in setCell");

    return (getCell(i, j) == Blank);
}

/*
* Prints the current board.
*/
void board::print()
{
    for (int i = 1; i <= BoardSize; i++)
    {
        if ((i - 1) % SquareSize == 0)
        {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++)
        {
            if ((j - 1) % SquareSize == 0)
                cout << "|";
            if (!isBlank(i, j))
                cout << " " << getCell(i, j) << " ";
            else
                cout << "   ";
        }
        cout << "|";
        cout << endl;
    }

    cout << " -";
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;
}

/*
* Prints the conflicts present in the current board.
*/
void board::printConflicts()
{
    cout << "The following initial conflicts were found: \n";
    for (int n = 0; n < 3; n++)
    {
        //choose which conflict matrix to print out
        matrix<ValueType> conflict_matrix;
        if (n == 0)
        {
            conflict_matrix = row_conflicts;
            cout << "Rows: \n";
        }
        else if (n == 1)
        {
            conflict_matrix = col_conflicts;
            cout << "Columns: \n";
        }
        else
        {
            conflict_matrix = square_conflicts;
            cout << "Squares: \n";
        }

        //print out conflict matrix
        for (int i = 1; i <= BoardSize; i++)
        {
            cout << "    -";
            for (int j = 1; j < BoardSize; j++)
                cout << "---";
            cout << "--";
            cout << endl;
            cout << i << ") ";
            for (int j = 1; j <= BoardSize; j++)
            {
                if (j == 1)
                    cout << "|";
                if (conflict_matrix[i][j] != Blank)
                    cout << " " << conflict_matrix[i][j] << " ";
                else
                    cout << "   ";
            }
            cout << "|";
            cout << endl;
        }
        cout << "    -";
        for (int j = 1; j < BoardSize; j++)
            cout << "---";
        cout << "--";
        cout << endl << endl;
    }
}
#pragma endregion Board

#pragma region NewHandlers

/*
* Update the conflicts on the board for given cell (r, c) and value v.
*/
void board::updateConflicts(int r, int c, int v, bool b)
{
    int mod;
    if (b)
    {
        mod = 1;
    }
    else
    {
        mod = Blank;
    }

    row_conflicts[r][v] = mod;
    col_conflicts[c][v] = mod;
    square_conflicts[squareNumber(r, c)][v] = mod;
}

/*
* Reset the given cell (r, c).
*/
void board::resetCell(int r, int c)
{
    int val = value[r][c];
    value[r][c] = Blank;
    updateConflicts(r, c, val, false);
}

/*
* Check whether or not the board is solved and return as a boolean.
*/
bool board::isSolved()
{
    for (int n = 0; n < 3; n++)
    {
        //choose which conflict matrix to print out
        matrix<ValueType> conflict_matrix;
        if (n == 0)
        {
            conflict_matrix = row_conflicts;
        }
        else if (n == 1)
        {
            conflict_matrix = col_conflicts;
        }
        else
        {
            conflict_matrix = square_conflicts;
        }

        //print out conflict matrix
        for (int i = 1; i <= BoardSize; i++)
        {
            for (int j = MinValue; j <= MaxValue; j++)
            {
                if (conflict_matrix[i][j] != 1)
                    return false;
            }
        }
    }
    return true;
}

bool board::numberIsLegal(vector<int> cell, int n)
{
    bool isLegal = true;

    //check whether value n is legal for row, col, and square.
    if (row_conflicts[cell[0]][n] == 1)
    {
        isLegal = false;
    }
    if (col_conflicts[cell[1]][n] == 1)
    {
        isLegal = false;
    }
    if (square_conflicts[squareNumber(cell[0], cell[1])][n] == 1)
    {
        isLegal = false;
    }

    return isLegal;
}

vector<ValueType> board::nextBlank()
{
    //return the next blank cell.
    vector<int> cell(2,0);
    int lock = -250;
    for (int r = 1; r <= BoardSize; r++)
    {
        for (int c = 1; c <= BoardSize; c++)
        {
            if (this->isBlank(r,c))
            {
                vector<int> comp(3,0);
                for (int n = 0; n < 3; n++)
                {
                    //choose which conflict matrix to print out
                    matrix<ValueType> conflict_matrix;
                    if (n == 0)
                    {
                        conflict_matrix = row_conflicts;
                        for (int i = 1; i <= BoardSize; i++)
                        {
                            comp[n] += conflict_matrix[r][i];
                        }
                        
                    }
                    else if (n == 1)
                    {
                        conflict_matrix = col_conflicts;
                        for (int j = 1; j<= BoardSize; j++)
                        {
                            comp[n] += conflict_matrix[c][j];
                        }
                    }
                    else
                    {
                        conflict_matrix = square_conflicts;
                        for (int k = 1; k <= BoardSize; k++)
                        {
                            comp[n] += conflict_matrix[squareNumber(r,c)][k];
                        }
                    }
                }
                int sum = comp[0] + comp[1] + comp[2];
                if (sum > lock)
                {
                    lock = sum;
                    cell[0] = r;
                    cell[1] = c;
                }
            }
        }
    }
    return cell;
}

/*
* Solves the board recursively.
*/
int solve(board board, int iter = 1)
{
    //first base case: the board is already solved.
    if (board.isSolved())
    {
        board.print();
        cout << "Iterations: " << iter << endl;
        return iter;
    }
    /*
    //second base case: no more legal choices for blank cell.
    //in this case, backtracking must happen.
    else if (cellFullyConstrained(i, j))
    {
        //backtrack
    }
    */
    //else try all values of n.
    else
    {
        // find first blank cell
        vector<int> blank = board.nextBlank();
        // Check All Numbers
        for (int n = 1; n <= 9; n++)
        {
            iter++;
            if (board.numberIsLegal(blank, n))
            {
                board.setCell(blank[0], blank[1], n);
                //board.print();
                //cout << "Setting " << n << " at " << blank[0] << "," << blank[1] << endl;
                solve(board, iter);
                board.resetCell(blank[0], blank[1]);
            }
        }
    }
}

#pragma endregion NewHandlers

/*
* The main function, where the code for the program is executed.
*/
int main()
{
    ifstream fin;
    string fileName = "sudoku1-3.txt";
    // Read the sample grid from the file.

    //Open the file, throw an error if it can't be opened.
    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }

    try
    {
        board b1(SquareSize);
        int iterations, avrg;
        int sum = 0;
        int probCount = 0;
        //Initialize the board.
        while (fin && fin.peek() != 'Z')
        {
            b1.initialize(fin);
            b1.print();
            //Try to solve the board
            cout << "\nWe will now attempt to solve the given board.\n";
            iterations = solve(b1);
            sum += iterations;
            probCount++;
        }
        avrg = sum / probCount;
        cout << "Average Iterations per Puzzle: " << avrg << endl;
    }
    catch (indexRangeError& ex)
    {
        cout << ex.what() << endl;
        exit(1);
    }
    fin.close();
}
