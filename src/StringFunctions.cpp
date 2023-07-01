#include "StringFunctions.h"
#include <sstream>

bool StringFunctions::starts_with(std::string s, std::string pattern)
{
	return s.rfind(pattern, 0) == 0;
}

std::string StringFunctions::trim(std::string s)
{
	return s.find_first_not_of(' ') != -1 ? s.substr(s.find_first_not_of(' '), s.find_last_not_of(' ') - s.find_first_not_of(' ') + 1) : "";
}

std::string StringFunctions::get_substring_after_char(std::string s, char c)
{
	return s.find(c) != -1 ? s.substr(s.find(c) + 1) : "";
}

bool StringFunctions::to_int(std::string input, int* output)
{
    return convert(input, output);
}

bool StringFunctions::to_int64(std::string input, int64_t* output)
{
    return convert(input, output);
}

bool StringFunctions::to_double(std::string input, double* output)
{
    return convert(input, output);
}

template <typename T>
bool StringFunctions::convert(std::string input, T* output)
{
    bool ok = true;
    std::stringstream ss;

    ss << input;
    ss >> *output;
    if (ss.fail()) ok = false;

    std::stringstream().swap(ss);

    return ok;
}