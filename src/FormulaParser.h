#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace Sheet {
class Spreadsheet; // Forward declaration
}
namespace utils{
    class FormulaParser {
    public:
        explicit FormulaParser(Sheet::Spreadsheet& spreadsheet);

        // Parses a formula and evaluates it
        double evaluateFormula(const std::string& formula);

    private:
        Sheet::Spreadsheet& spreadsheet;

        // Helper functions
        double getCellValue(const std::string& cellReference);
        std::vector<std::string> tokenize(const std::string& formula);
        double computeOperation(double left, const std::string& op, double right);
        double computeFunction(const std::string& function, const std::string& range);
    };
}
#endif // FORMULA_PARSER_H
