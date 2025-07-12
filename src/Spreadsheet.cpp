/*
* Spreadsheet class is the controller class of the project.
* It uses Cell classes , FormulaParser ,AnsiTerminal ,and FileManager classes
* to-do list:
* printign mechanism: done
* grid extending : done 
* file operations (save, save as, load): done
* navigation in grid: done
* Arithmetic operation(+,-,*,/): done
* Cell refrencing: 
* formula functions (SUM,AVVER,MAX,MIN,STDDEV)
*/

#include "Spreadsheet.h"
#include "FormulaParser.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>

using std::string;
namespace Sheet {

/**
 * Constructs a Spreadsheet object with the specified number of rows and columns.
 * 
 * @param rows Number of rows in the grid.
 * @param cols Number of columns in the grid.
 */
Spreadsheet::Spreadsheet(int rows, int cols)
    : rows(rows), cols(cols), grid(new std::unique_ptr<std::unique_ptr<Cell>[]>[rows]),
      fileManager(std::make_unique<utils::FileManager>()) {
    for (int i = 0; i < rows; ++i) {
        grid[i] = std::make_unique<std::unique_ptr<Cell>[]>(cols);
    }
}
/**
 * Resizes the grid to new dimensions, preserving existing data.
 * 
 * @param newRows New number of rows.
 * @param newCols New number of columns.
 */
void Spreadsheet::resizeGrid(int newRows, int newCols) {
    auto newGrid = std::make_unique<std::unique_ptr<std::unique_ptr<Cell>[]>[]>(newRows);
    for (int i = 0; i < newRows; ++i) {
        newGrid[i] = std::make_unique<std::unique_ptr<Cell>[]>(newCols);
        if (i < rows) {
            for (int j = 0; j < cols; ++j) {
                newGrid[i][j] = std::move(grid[i][j]);
            }
        }
    }
    rows = newRows;
    cols = newCols;
    grid = std::move(newGrid);
}
/*
* return the current row number of the grid
*/
int Spreadsheet::getRowCount() const {
    return rows;
}
/*
* return the current columuns number of the grid
*/
int Spreadsheet::getColumnCount() const {
    return cols;
}
/*
* return the current opened file name.
*/
const string& Spreadsheet::getOpenedFile() const {
    return openedFile;
}
/**
 * Sets the name of the currently opened file.
 * 
 * @param fileName The new file name.
 */
void Spreadsheet::setOpenedFile(const string& fileName) {
    openedFile = fileName;
}
/**
 * Retrieves the content of the specified cell, evaluating formulas if present.
 * 
 * @param row Row index of the cell.
 * @param col Column index of the cell.
 * @return String representation of the cell's content or "Error" if evaluation fails.
 */
const string Spreadsheet::getCell(int row, int col) const {
    if (row >= rows || col >= cols) throw std::out_of_range("Invalid cell access.");
    if (grid[row][col]) {
        string cellValue = grid[row][col]->getCell();
        if (!cellValue.empty() && cellValue[0] == '=') {
            utils::FormulaParser parser(const_cast<Spreadsheet&>(*this)); // Remove constness for parsing
            try {
                double result = parser.evaluateFormula(cellValue);
                return std::to_string(result); // Return the evaluated result
            } catch (const std::exception& e) {
                return "Error"; // Return error if formula evaluation fails
            }
        }
        return cellValue; // Return non-formula cell value
    }
    return "";
}

/**
 * Updates the content of the specified cell.
 * 
 * @param row Row index where the cell is located.
 * @param col Column index where the cell is located.
 * @param value New content to be set in the cell.
 */
void Spreadsheet::setCell(int row, int col, const string& value) {
    if (row >= rows || col >= cols) {
        resizeGrid(std::max(rows, row + 1), std::max(cols, col + 1));
    }

    if (!value.empty() && value[0] == '=') {
        // Evaluate the formula
        utils::FormulaParser parser(static_cast<Spreadsheet&>(*this));
        try {
            double result = parser.evaluateFormula(value);
            grid[row][col] = std::make_unique<DoubleValueCell>(std::to_string(result));
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            grid[row][col] = std::make_unique<StringValueCell>(value);
        }
    } else {
        grid[row][col] = std::make_unique<StringValueCell>(value);
    }

    // todo
    // recalculateDependencies(row, col);
}
/**
 * Displays the spreadsheet grid on the terminal, starting from the specified row and column.
 * 
 * @param startRow Starting row index for display.
 * @param startCol Starting column index for display.
 */
void Spreadsheet::displayGrid(int startRow, int startCol) {
    terminal.clearScreen();
    terminal.printAt(1, 5, "        ");
    int endCol = std::min(startCol + maxVisibleCols, cols);
    for (int col = startCol; col < endCol; ++col) {
        terminal.printAt(1, (col - startCol) * 15 + 5, string(1, 'A' + col));
    }

    int endRow = std::min(startRow + maxVisibleRows, rows);
    for (int row = startRow; row < endRow; ++row) {
        terminal.printAt(row - startRow + 2, 1, std::to_string(row + 1));
        for (int col = startCol; col < endCol; ++col) {
            string cellValue = getCell(row, col);
            terminal.printAt(row - startRow + 2, (col - startCol) * 15 + 5, cellValue.empty() ? "             " : cellValue);
        }
    }
    terminal.printAt(maxVisibleRows + 2, 1, "Use Arrow Keys to Navigate, Enter to Edit, 'A' to Save as, 'S' to Save, 'Q' to Quit.");
}

/**
 * Edits the content of the currently selected cell.
 * 
 * @param row Row index of the cell to be edited.
 * @param col Column index of the cell to be edited.
 */
void Spreadsheet::editCell(int row, int col) {
    terminal.printAt(maxVisibleRows + 5, 1, "Enter new value: ");
    string newValue;
    std::getline(std::cin, newValue);
    setCell(row, col, newValue);
    terminal.printAt(maxVisibleRows + 5, 1, "");
}
/*
* Saves the current grid in csv file using the save method of the FileManager class
* If no file opened the save will not work and the user will be asked to use save as option
*/
void Spreadsheet::saveCSVFile() {
    if (openedFile.empty()) {
        terminal.printAt(maxVisibleRows + 3, 1, "No file currently open. Use 'A' for Save As and enter a file name.");
        return;
    }
    fileManager->save(*this, openedFile);
}
/*
* Asks the user to enter the file name to to save the current grid in csv file using save method of the FileManager class
* sets the curently opened file to the file name entered using setOpenedFile
*/
void Spreadsheet::saveAsCSVFile() {
    string newFilename;
    terminal.printAt(maxVisibleRows + 3, 1, "Enter a new file name to save as: ");
    std::cin >> newFilename;
    std::cin.ignore();
    while (newFilename.substr(newFilename.find_last_of(".") + 1) != "csv") {
        string error = "Error: Invalid file name. Please enter a valid file name with a '.csv' extension.";
        terminal.printAt(maxVisibleRows + 2, 1, error);

        terminal.printAt(maxVisibleRows + 3, 1, "Enter a new file name ends with .csv to save as: ");
        std::cin >> newFilename;
        std::cin.ignore();
    }
    fileManager->save(*this, newFilename); 
    setOpenedFile(newFilename);
}
/*
* Asks the user for a file name to load its contents to the grid using load method of the FileManager class
*/
void Spreadsheet::loadCSVFile() {
    string filename;
    terminal.printAt(maxVisibleRows + 2, 1, "Enter the file name to load: ");
    std::cin >> filename;
    std::cin.ignore();
    if (filename.substr(filename.find_last_of(".") + 1) != "csv") {
        filename += ".csv";
    }
    fileManager->load(*this, filename);
}

void Spreadsheet::startProgram() {
    int currentRow = 0;
    int currentCol = 0;
    int startRow = 0;
    int startCol = 0;
    char key;

    terminal.clearScreen();
    char choice;
    std::cout << "=============================================== " << std::endl;
    std::cout << "      Welcome to GTU Spreadsheet Program v2     " << std::endl;
    std::cout << "=============================================== " << std::endl;
    std::cout << "1. Load Spreadsheet from file" << std::endl;
    std::cout << "2. Start with an empty spreadsheet" << std::endl;
    std::cout << "Q. Exit the program" << std::endl;
    std::cout << "Enter your choice : ";
    std::cin >> choice;
    std::cin.ignore();

    while (choice != '1' && choice != '2' && choice != 'q' && choice != 'Q') {
        std::cerr << "Invalid choice. Please enter '1' or '2': \n";
        std::cin >> choice;
        std::cin.ignore();
    }
    if(choice == 'q' || choice == 'Q'){
        terminal.clearScreen();
        std::cout << "Exiting program byeee..." << std::endl;
        return;
    }
    else if (choice == '1') {
        loadCSVFile();
    } else {
        rows = 30;
        cols = 12;
        grid = std::make_unique<std::unique_ptr<std::unique_ptr<Cell>[]>[]>(rows);
        for (int i = 0; i < rows; ++i) {
            grid[i] = std::make_unique<std::unique_ptr<Cell>[]>(cols);
        }
        openedFile.clear();
    }

    // main loop for navigating and editing
    while (true) {
        displayGrid(startRow, startCol);
        string currentCellValue = getCell(currentRow, currentCol);
        terminal.printInvertedAt(currentRow - startRow + 2, (currentCol - startCol) * 15 + 5, currentCellValue.empty() ? "             " : currentCellValue);

        key = terminal.getSpecialKey();
        switch (key) {
            case 'J':
            case '\n':
            case 'e':
            case 'E':
                editCell(currentRow, currentCol);
                break;
            case 'U':
                if (currentRow > 0) {
                    --currentRow;
                    if (currentRow < startRow) {
                        --startRow;
                    }
                }
                break;
            case 'D':
                if (currentRow >= rows - 1) resizeGrid(rows + 1, cols);
                ++currentRow;
                if (currentRow >= startRow + maxVisibleRows) {
                    ++startRow;
                }
                break;
            case 'L':
                if (currentCol > 0) {
                    --currentCol;
                    if (currentCol < startCol) {
                        --startCol;
                    }
                }
                break;
            case 'R':
                if (currentCol >= cols - 1) resizeGrid(rows, cols + 1);
                ++currentCol;
                if (currentCol >= startCol + maxVisibleCols) {
                    ++startCol;
                }
                break;
            case 'A':
            case 'a':
                saveAsCSVFile();
                break;
            case 'S':
            case 's':
                saveCSVFile();
                break;
            case 'Q':
            case 'q':
                terminal.clearScreen();
                std::cout << "Exiting program byeee..." << std::endl;
                return;
            default:
                terminal.printAt(maxVisibleRows + 4, 1, "Invalid option.Pleas enter a valid option(Enter to Edit, 'A' to Save as, 'S' to Save, 'Q' to Quit)");
        }
    }
}

} // namespace Sheet
