#include "FileParser.h"
#include <iostream>
#include <fstream>
using namespace std;


	unsigned int FileParser::ReadFile(const char* fileName, LINESARRAY& outLines)
	{
		string line;
		ifstream myfile (fileName);
		unsigned int numLinesRead = 0;

		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				outLines.push_back(line);
				++numLinesRead;
			}

			myfile.close();
		}
		else
		{
			//give error msg about not able to open file
			return numLinesRead;
		}
	}

	bool FileParser::WriteToFile(const char* fileName, const LINESARRAY& inLines)
	{
		ofstream myfile (fileName);
		if (myfile.is_open())
		{
			for(LINESARRAY::const_iterator it = inLines.begin(); it != inLines.end();++it)
			{
				myfile << (*it);
			}

			myfile.close();
		}
		else
		{
			//give error about file not open
			return false;
		}

		return true;
	}
