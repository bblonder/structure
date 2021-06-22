#ifndef _STRING_HELPER_H_
#define _STRING_HELPER_H_

#include <string>
#include <vector>

std::string getString(std::string src, unsigned int colstart, unsigned int colend);
float getFloat(std::string src, unsigned int colstart, unsigned int colend);
float getInt(std::string src, unsigned int colstart, unsigned int colend);
std::string extractValue(std::string arg, std::string key);
std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters);
std::string removeWhitespace(std::string arg, std::string delims, std::string newSpacer);

#endif