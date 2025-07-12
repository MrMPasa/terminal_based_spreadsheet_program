#ifndef CELL_H
#define CELL_H

#include <string>

using std::string;
namespace Sheet{
    class Cell{
    public:
        Cell() = default;
        virtual ~Cell() = default;
        
        virtual const string getCell()const = 0;
        virtual void setCell(const string& newCell) = 0;
    };//end of class Cell
}
#endif
