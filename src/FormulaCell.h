#ifndef FORMULACELL_H
#define FORMULACELL_H

#include <string>
#include "Cell.h"

using std::string;

namespace Sheet{
    class FormulaCell : public Cell{
        private:
            string formula;
        public:
            FormulaCell();
            FormulaCell(const string& newValue);
            ~FormulaCell() = default;

            const string getCell()const override;
            void setCell(const string& newValue) override;
    };//end of class FormulaCell
}
#endif
