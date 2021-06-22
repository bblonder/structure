#include "StringHelper.h"

std::string getString(std::string src, unsigned int colstart, unsigned int colend)
{
	if (src.size() >= colend && colstart >= 1)
	{
		return src.substr(colstart - 1, colend - colstart + 1);
	}
	else
	{
		return "";
	}
}

float getFloat(std::string src, unsigned int colstart, unsigned int colend)
{
	return atof(getString(src, colstart, colend).c_str());
}

float getInt(std::string src, unsigned int colstart, unsigned int colend)
{
	return atoi(getString(src, colstart, colend).c_str());
}

std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters)
{
	std::vector<std::string> tokens;

    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
	
	return tokens;
}


std::string removeWhitespace(std::string arg, std::string delims, std::string newSpacer)
{
	std::vector<std::string> tokens = tokenize(arg, delims);
	std::string result = "";
	
	for (int i=0; i<tokens.size(); i++)
	{
		if (i != 0)
		{
			result += newSpacer;
		}
		result += tokens.at(i);
	}
	
	return result;
}

std::string extractValue(std::string arg, std::string key)
{
	int pos = arg.find(key);
	
	if (pos != std::string::npos)
	{
		return arg.substr(pos + key.size(), std::string::npos);
	}
	else return "";
}