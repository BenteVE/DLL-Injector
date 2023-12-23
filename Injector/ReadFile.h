#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

std::string CutString(std::string line);

bool FileExists(std::string fileName);

void ReadSettingsFile(std::string fileName, std::string& firstLine, std::string& secondLine, std::string& thirdLine);