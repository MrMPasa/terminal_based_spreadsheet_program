#include <iostream>
#include <string>
#include <exception>
#include "StringValueCell.h"

using std::string;
namespace Sheet{
    StringValueCell::StringValueCell(const string& newValue){
        stringValue = newValue;
    }
    const string StringValueCell::getCell()const{
        return stringValue;
    }
    void StringValueCell::setCell(const string& newValue){
        stringValue = newValue;
    }
}