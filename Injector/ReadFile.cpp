#include "ReadFile.h"
#include "Error.h"

std::string CutString(std::string line) {
    unsigned first = line.find("\"") + 1;
    unsigned last = line.find_last_of("\"");
    return line.substr(first, last - first);
}

bool FileExists(std::string fileName)
{
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0); //if function returns 0, the function worked => the file exists
}

void ReadSettingsFile(std::string fileName, std::string& firstLine, std::string& secondLine, std::string& thirdLine) {

    if (!FileExists(fileName))
    {
        DisplayError("fileExists", "Settings File doesn't exist");
    }

    std::ifstream inputFile(fileName);
    if (inputFile.is_open()) {
        std::string line;

        getline(inputFile, line);
        firstLine = CutString(line);

        getline(inputFile, line);
        secondLine = CutString(line);

        getline(inputFile, line);
        thirdLine = CutString(line);

        inputFile.close();
    }
}