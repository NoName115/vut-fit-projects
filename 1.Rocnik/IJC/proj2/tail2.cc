/*2.DU na predmet IJC
* VUT - FIT
* Róbert Kolcún - xkolcu00
* Program pre ulohu A)
* 24.5.2016
*/

#include <iostream>
#include <queue>
#include <string>

#include <fstream>
#include <istream>
#include <sstream>

#define defaultNumberOfLines 10

using namespace std;

queue<string> stringBuffer;

ifstream file;

//IsDigit
bool IsDigits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

int main(int argc, char **argv)
{
	std::ios::sync_with_stdio(false);

	bool readingFromFile = false;
	unsigned numberOfLines = 0;

	stringstream ss;

	if (argc > 4)
	{
		fprintf(stderr, "%s\n", "Error: Too many arguments");
		return 1;
	}

	if (argc == 1)
	{
		numberOfLines = defaultNumberOfLines;
	}

	if (argc == 2)
	{
		file.open(argv[1]);

		if (!file.is_open())
		{
			fprintf(stderr, "%s\n", "Error: Opening file");
			return 1;
		}

		numberOfLines = defaultNumberOfLines;
		readingFromFile = true;
	}

	if (argc == 3)
	{
		ss.str(argv[1]);
		if (ss.str() == "-n")
		{
			//IfDigit condition
			if (IsDigits(argv[2]))
			{
				ss.str(argv[2]);
				ss >> numberOfLines;
			} else
			{
				fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[2]);
				return 1;
			}
		} else
		{
			fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[1]);
			return 1;
		}
	}

	if (argc == 4)
	{
		ss.str(argv[1]);
		if (ss.str() == "-n")
		{
			//IfDigit condition
			if (IsDigits(argv[2]))
			{
				ss.str(argv[2]);
				ss >> numberOfLines;

				file.open(argv[3]);
				if (!file.is_open())
				{
					fprintf(stderr, "%s\n", "Error: Opening file");
					return 1;
				}

				readingFromFile = true;
			} else
			{
				fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[2]);
				return 1;
			}
		} else
		{
			fprintf(stderr, "%s %s\n", "Error: Wrong argument", argv[1]);
			return 1;
		}
	}

	if (readingFromFile)
	{
		string pomString;
		while (getline(file, pomString))
		{	
			stringBuffer.push(pomString);
			if (stringBuffer.size() > numberOfLines)
			{
				stringBuffer.pop();
			}
		}
	} else
	{
		string pomString;
		while (getline(cin, pomString))
		{
			stringBuffer.push(pomString);
			if (stringBuffer.size() > numberOfLines)
			{
				stringBuffer.pop();
			}
		}
	}

	while (!stringBuffer.empty())
	{
		cout << stringBuffer.front() << endl;
		stringBuffer.pop();
	}

	return 0;
}
