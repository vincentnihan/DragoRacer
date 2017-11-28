#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>

class FileParser
{
public:
	typedef std::vector<std::string> LINESARRAY;
	static unsigned int ReadFile(const char* fileName, LINESARRAY& outLines);
	static bool WriteToFile(const char* fileName, const LINESARRAY& inLines);
};

#endif