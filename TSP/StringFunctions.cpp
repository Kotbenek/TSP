#include "StringFunctions.h"
#include <sstream>

bool StringFunctions::starts_with(string s, string pattern)
{
	return s.rfind(pattern, 0) == 0;
}

string StringFunctions::trim(string s)
{
	return s.find_first_not_of(' ') != -1 ? s.substr(s.find_first_not_of(' '), s.find_last_not_of(' ') - s.find_first_not_of(' ') + 1) : "";
}

string StringFunctions::get_substring_after_char(string s, char c)
{
	return s.find(c) != -1 ? s.substr(s.find(c) + 1) : "";
}

bool StringFunctions::to_int(string input, int* output)
{
    return convert(input, output);
}

bool StringFunctions::to_int64(string input, int64_t* output)
{
    return convert(input, output);
}

bool StringFunctions::to_double(string input, double* output)
{
    return convert(input, output);
}

template <typename T>
bool StringFunctions::convert(string input, T* output)
{
    bool ok = true;
    stringstream ss;

    ss << input;
    ss >> *output;
    if (ss.fail()) ok = false;

    stringstream().swap(ss);

    return ok;
}