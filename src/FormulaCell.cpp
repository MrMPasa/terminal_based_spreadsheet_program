#include <string>
#include <exception>
#include <stdexcept>
#include "FormulaCell.h"

using std::string;

namespace Sheet{
    FormulaCell::FormulaCell(const string& newValue){
        if(newValue.at(0) == '=')
            formula = newValue;
        else
            throw std::invalid_argument("not a formula");
    }
    const string FormulaCell::getCell()const{
        return formula;
    }
    void FormulaCell::setCell(const string& newValue){
        formula = newValue;
    }
}