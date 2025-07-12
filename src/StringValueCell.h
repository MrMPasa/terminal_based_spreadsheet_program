#ifndef STRINGVALUECELL_H
#define STRINGVALUECELL_H

#include "ValueCell.h"

using std::string;
namespace Sheet {
    class StringValueCell : public ValueCell {
    private:
        string stringValue;

    public:
        StringValueCell();
        StringValueCell(const string& newValue);
        ~StringValueCell() = default;

        void setCell(const string& newValue) override;
        const string getCell()const override;

        const string& getCellValue()const;
    };
} // namespace Sheet

#endif
