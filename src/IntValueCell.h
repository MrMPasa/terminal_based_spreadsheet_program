#ifndef INTVALUECELL_H
#define INTVALUECELL_H

#include "ValueCell.h"

using std::string;
namespace Sheet {

    class IntValueCell : public ValueCell {
    private:
        int intValue;

    public:
        IntValueCell();
        IntValueCell(const string& newValue);
        ~IntValueCell() = default;

        void setCell(const string& newValue) override;
        const string getCell()const override;

        int getCellValue()const;
    };
} // namespace Sheet

#endif
