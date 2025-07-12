#ifndef DOUBLEVALUECELL_H
#define DOUBLEVALUECELL_H

#include "ValueCell.h"

using std::string;
namespace Sheet {

    class DoubleValueCell : public ValueCell {
    private:
        double doubleValue;

    public:
        DoubleValueCell();
        DoubleValueCell(const string& newValue);
        ~DoubleValueCell() = default;

        void setCell(const string& newValue) override;
        const string getCell()const override;

        double getCellValue()const;
    };
} // namespace Sheet

#endif
