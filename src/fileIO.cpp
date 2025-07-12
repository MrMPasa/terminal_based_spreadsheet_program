#include "fileIO.h"
#include "Spreadsheet.h"
#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;
using std::cerr;
namespace utils{
    void FileManager::save(Sheet::Spreadsheet& sps, const std::string& fileName) {
        string targetFile = fileName.empty() ? sps.getOpenedFile() : fileName;
        if (targetFile.empty()) {
            string error = "Error: No file name provided to save the file.";
            sps.terminal.printAt(sps.maxVisibleRows + 3, 1, error);
            return;
        }

        std::ofstream outputFile(targetFile);
        if (!outputFile.is_open()) {
            string error = "Error opening file '" + targetFile + "' for writing.";
            sps.terminal.printAt(sps.maxVisibleRows + 3, 1, error);
            return;
        }

        for (int i = 0; i < sps.getRowCount(); ++i) {
            for (int j = 0; j < sps.getColumnCount(); ++j) {
                std::string cellValue = sps.getCell(i, j);
                outputFile << cellValue;
                if (j != sps.getColumnCount() - 1) outputFile << ",";
            }
            outputFile << '\n';
        }
        outputFile.close();
        string msg = "File saved successfully to '" + fileName + "'.\n";
        sps.terminal.printAt(sps.maxVisibleRows + 5, 1,msg);
    }

    void FileManager::load(Sheet::Spreadsheet& sps,std::string& fileName) {
        std::ifstream inputFile(fileName);
        while (!inputFile.is_open()) {
            string error = "Error opening \"" + fileName + "\" file for reading.";
            sps.terminal.printAt(sps.maxVisibleRows + 2, 1, error);

            string prompt = "Enter the file name again or 'N' to start a new sheet:";
            sps.terminal.printAt(sps.maxVisibleRows + 3, 1, prompt);

            std::cin >> fileName;
            std::cin.ignore();
     
            inputFile.open(fileName);
            if(fileName.length() == 1 && (fileName.at(0) == 'n' || fileName.at(0) == 'N'))
                return;

            if (fileName.substr(fileName.find_last_of(".") + 1) != "csv") {
                fileName += ".csv";
            }
        }
        sps.setOpenedFile(fileName);
        std::string line;
        int rowIndex = 0;

        while (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            std::string cellValue;
            int colIndex = 0;

            while (std::getline(ss, cellValue, ',')) {
                sps.setCell(rowIndex, colIndex, cellValue);
                ++colIndex;
            }
            ++rowIndex;
        }
        inputFile.close();
        string msg = "File loaded successfully from '" + fileName + "'.\n";
        sps.terminal.printAt(sps.maxVisibleRows + 5, 1,msg);
    }
}