#include <iostream>
#include <string>
#include <exception>
#include "IntValueCell.h"

using std::string;
using std::exception;
using std::cout;
namespace Sheet{
    IntValueCell::IntValueCell(const string& newValue){
        try{
            intValue = std::stoi(newValue);
        }catch(exception& e){
            cout<< "Invalid cell content!\nFailed converting string to integer.\nCaller: IntValueCell()\nError code: " << e.what() << std::endl;
        }
    }
    const string IntValueCell::getCell()const{
        return std::to_string(intValue);
    }
    void IntValueCell::setCell(const string& newValue){
        try{
            intValue = std::stoi(newValue);
        }catch(exception& e){
            cout<< "Invalid cell content!\nFailed converting string to integer.\nCaller: IntValueCell.setCell()\nError code: " << e.what() << std::endl;
        }
    }
    int IntValueCell::getCellValue()const{
        return intValue;
    }
}