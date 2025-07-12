#include <iostream>
#include <string>
#include <exception>
#include "DoubleValueCell.h"

using std::string;
using std::exception;
using std::cout; 

namespace Sheet{
    DoubleValueCell::DoubleValueCell(const string& newValue){
        try{
            doubleValue = std::stod(newValue);
        }catch(exception& e){
            cout<< "Invalid cell content!\nFailed converting string to double.\nCaller: DoubleValueCell()\nError code: " << e.what() << std::endl;
        }
    }
    const string DoubleValueCell::getCell()const{
        return std::to_string(doubleValue);
    }
    void DoubleValueCell::setCell(const string& newValue){
        try{
            doubleValue = std::stod(newValue);
        }catch(exception& e){
            cout<< "Invalid cell content!\nFailed converting string to double.\nCaller: DoubleValueCell.setCell()\nError code: " << e.what() << std::endl;
        }
    }
    double DoubleValueCell::getCellValue()const{
        return doubleValue;
    }
}