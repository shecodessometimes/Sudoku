#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include "board.h"
#include <list>
#include <math.h>
#include <fstream>

#pragma region Board
board::board(int sqSize)
    : value(BoardSize + 1, BoardSize + 1)
    // Board constructor
{
    row_conflicts = matrix<int>(BoardSize + 1, BoardSize + 1);
    col_conflicts = matrix<int>(BoardSize + 1, BoardSize + 1);
    square_conflicts = matrix<int>(BoardSize + 1, BoardSize + 1);
    clear();
}

void board::clear()
// Mark all possible values as legal for each board entry
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

void board::initialize(ifstream& fin)
// Read a Sudoku board from the input file.
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

void board::setCell(int r, int c, int n)
//Set the value at cell at index (r, c) to n.
{
    value[r][c] = n;
    updateConflicts(r, c, n, true);
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
    // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
    // coordinates of the square that i,j is in.  

    return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream& operator<<(ostream& ostr, vector<int>& v)
// Overloaded output operator for vector class.
{
    for (int i = 0; i < v.size(); i++)
        ostr << v[i] << " ";
    cout << endl;
    return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        return value[i][j];
    else
        throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("bad value in setCell");

    return (getCell(i, j) == Blank);
}

void board::print()
// Prints the current board.
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

void board::printConflicts()
// Prints the current board.
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

void board::updateConflicts(int r, int c, int v, bool b)
// 
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

void board::resetCell(int r, int c)
{
    int val = value[r][c];
    value[r][c] = Blank;
    updateConflicts(r, c, val, false);
}

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

#pragma endregion NewHandlers

int main()
{
    ifstream fin;
    string fileName;
    // Read the sample grid from the file.
    for (int i = 1; i <= 3; i++)
    {
        if (i == 1)
        {
            fileName = "sudoku1.txt";
        }
        else if (i == 2)
        {
            fileName = "sudoku2.txt";
        }
        else
        {
            fileName = "sudoku3.txt";
        }
        fin.open(fileName.c_str());
        if (!fin)
        {
            cerr << "Cannot open " << fileName << endl;
            exit(1);
        }

        try
        {
            board b1(SquareSize);

            while (fin && fin.peek() != 'Z')
            {
                b1.initialize(fin);
                b1.print();
                b1.printConflicts();
            }

            if (b1.isSolved())
            {
                cout << "Perfect Input!" << endl;
            }
            else
            {
                cout << "Not Quite" << endl;
            }
        
        }
        catch (indexRangeError& ex)
        {
            cout << ex.what() << endl;
            exit(1);
        }
        fin.close();
    }
}