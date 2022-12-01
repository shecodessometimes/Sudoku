// Declarations and functions for project #4
#pragma once

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class board
    // Stores the entire Sudoku board
{
public:
    board(int);
    void clear();
    void initialize(ifstream& fin);
    void print();
    void printConflicts();
    bool isBlank(int, int);
    ValueType getCell(int, int);
    void setCell(int, int, int);
    void updateConflicts(int, int, int, bool);
    void resetCell(int, int);
    bool isSolved();
    //void solve();
    //bool cellFullyConstrained(int i, int j);
    bool numberIsLegal(std::vector<int> cell, int n);
    vector<ValueType> nextBlank();
private:

    // The following matrices go from 1 to BoardSize in each
    // dimension, i.e., they are each (BoardSize+1) * (BoardSize+1)

    matrix<ValueType> value;
    matrix<ValueType> row_conflicts;
    matrix<ValueType> col_conflicts;
    matrix<ValueType> square_conflicts;
};