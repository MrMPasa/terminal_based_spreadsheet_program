#ifndef FILES_H
#define FILES_H

#include <string>

namespace Sheet {
    class Spreadsheet; // Forward declaration
}
namespace utils{
    class FileManager {
    public:
        void save(Sheet::Spreadsheet& sps, const std::string& fileName);
        void load(Sheet::Spreadsheet& sps, std::string& fileName);
    };
}
#endif
