#ifndef VALUECELL_H
#define VALUECELL_H

#include <string>
#include "Cell.h"

using std::string;
namespace Sheet{
    class ValueCell : public Cell{
    public:
        ValueCell() = default;
        virtual ~ValueCell() = default;

        const string getCell()const override = 0;
        void setCell(const string& newCell) override = 0;
    };//end of class VALUECELL_H
}
#endif
