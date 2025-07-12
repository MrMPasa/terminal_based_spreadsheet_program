#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <iostream>
#include <memory>
#include <string>
#include "AnsiTerminal.h"
#include "Cell.h"
#include "IntValueCell.h"
#include "StringValueCell.h"
#include "DoubleValueCell.h"
#include "FormulaCell.h"
#include "fileIO.h"

using std::string;
namespace Sheet {

class Spreadsheet {
private:
    int rows;
    int cols;
    std::unique_ptr<std::unique_ptr<std::unique_ptr<Cell>[]>[]> grid;
    std::unique_ptr<utils::FileManager> fileManager;
    string openedFile;

    void resizeGrid(int newRows, int newCols);

public:
    Spreadsheet(int rows = 30, int cols = 12);

    void displayGrid(int startRow, int startCol);
    void editCell(int row, int col);
    void startProgram();

    void saveCSVFile();
    void saveAsCSVFile();
    void loadCSVFile();

    int getRowCount() const;
    int getColumnCount() const;

    const string& getOpenedFile() const;
    void setOpenedFile(const string& fileName);

    const string getCell(int row, int col) const;
    void setCell(int row, int col, const string& value);

    //todo
    //void recalculateDependencies(int,int);
    
    utils::AnsiTerminal terminal;
    const int maxVisibleRows = 39;
    const int maxVisibleCols = 12;
};

} // namespace Sheet

#endif // SPREADSHEET_H
