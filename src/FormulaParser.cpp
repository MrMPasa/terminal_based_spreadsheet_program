#include "FormulaParser.h"
#include "Spreadsheet.h"
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <regex>
#include <cmath>
namespace utils{
    FormulaParser::FormulaParser(Sheet::Spreadsheet& spreadsheet)
        : spreadsheet(spreadsheet) {}

    double FormulaParser::evaluateFormula(const std::string& formula) {
        if (formula.empty() || formula[0] != '=') {
            throw std::invalid_argument("Invalid formula format");
        }

        std::string expression = formula.substr(1); // Remove leading '='
        auto tokens = tokenize(expression);

        std::vector<double> values;
        std::vector<std::string> operators;

        for (const auto& token : tokens) {
            if ((token[0] >= '0' && token[0] <= '9') || token[0] == '.') {
                values.push_back(std::stod(token));
            } else if ((token[0] >= 'A' && token[0] <= 'Z') || (token[0] >= 'a' && token[0] <= 'z')) {
                values.push_back(getCellValue(token));
            } else if (token == "+" || token == "-" || token == "*" || token == "/") {
                operators.push_back(token);
            } else if (token.find("(") != std::string::npos) {
                std::string function = token.substr(0, token.find("("));
                std::string range = token.substr(token.find("(") + 1, token.find(")") - token.find("(") - 1);
                values.push_back(computeFunction(function, range));
            }
        }

        // Evaluate the operations in order (no operator precedence, left-to-right evaluation)
        while (!operators.empty()) {
            double left = values[0];
            double right = values[1];
            std::string op = operators[0];

            values.erase(values.begin());
            values[0] = computeOperation(left, op, right);
            operators.erase(operators.begin());
        }

        return values.empty() ? 0.0 : values[0];
    }

    double FormulaParser::getCellValue(const std::string& cellReference) {
        size_t row = std::stoul(cellReference.substr(1)) - 1;
        size_t col = cellReference[0] - 'A';

        std::string cellValue = spreadsheet.getCell(row, col);
        if (cellValue.empty()) {
            return 0.0;
        }

        // Check if the cell contains a formula
        if (cellValue[0] == '=') {
            try {
                return evaluateFormula(cellValue); // Re-evaluate the formula
            } catch (...) {
                throw std::invalid_argument("Invalid formula in cell: " + cellReference);
            }
        }

        // Otherwise, treat it as a numeric value or return 0
        try {
            return std::stod(cellValue);
        } catch (...) {
            return 0.0; // Non-numeric cells default to 0
        }
    }


    std::vector<std::string> FormulaParser::tokenize(const std::string& formula) {
    // Regular expression to match numbers, operators, cell references, and function calls with their parameters
    std::regex tokenRegex(
        R"(\b(\d+(\.\d*)?|\.\d+)|([A-Za-z_]\w*)|([+*/()-])|\b)"); // Matches numbers, identifiers, and operators

    std::vector<std::string> tokens;
    std::smatch match;

    // Iterate over all matches that conform to the regex
    std::string::const_iterator searchStart(formula.cbegin());
    while (regex_search(searchStart, formula.cend(), match, tokenRegex)) {
        std::string token = match[0];
        if (!token.empty() && token != " ") { // Ignore spaces if they are matched
            tokens.push_back(token);
        }
        searchStart = match.suffix().first; // Continue from the end of the last match
    }

    return tokens;
}



    double FormulaParser::computeOperation(double left, const std::string& op, double right) {
        if (op == "+") {
            return left + right;
        } else if (op == "-") {
            return left - right;
        } else if (op == "*") {
            return left * right;
        } else if (op == "/") {
            if (right == 0) {
                throw std::runtime_error("Division by zero");
            }
            return left / right;
        }

        throw std::invalid_argument("Invalid operator: " + op);
    }

    double FormulaParser::computeFunction(const std::string& function, const std::string& range) {
        // Parse range (e.g., "A1..A10")
        size_t startRow = std::stoul(range.substr(1, range.find("..") - 1)) - 1;
        size_t startCol = range[0] - 'A';

        size_t endRow = std::stoul(range.substr(range.find("..") + 3)) - 1;
        size_t endCol = range[range.find("..") + 2] - 'A';

        std::vector<double> values;
        for (size_t row = startRow; row <= endRow; ++row) {
            for (size_t col = startCol; col <= endCol; ++col) {
                std::string cellValue = spreadsheet.getCell(row, col);
                if (!cellValue.empty()) {
                    try {
                        values.push_back(std::stod(cellValue));
                    } catch (...) {
                        throw std::invalid_argument("Invalid cell value in range: " + cellValue);
                    }
                }
            }
        }

        // Compute based on the function
        if (function == "SUM") {
            return std::accumulate(values.begin(), values.end(), 0.0);
        } else if (function == "AVER") {
            return values.empty() ? 0.0 : std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        } else if (function == "STDDEV") {
            double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
            double sumSquaredDiff = 0.0;
            for (const auto& value : values) {
                sumSquaredDiff += (value - mean) * (value - mean);
            }
            return std::sqrt(sumSquaredDiff / values.size());
        } else if (function == "MAX") {
            return *std::max_element(values.begin(), values.end());
        } else if (function == "MIN") {
            return *std::min_element(values.begin(), values.end());
        }

        throw std::invalid_argument("Invalid function: " + function);
    }
}