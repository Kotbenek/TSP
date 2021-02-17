#include "StringFunctions.h"

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