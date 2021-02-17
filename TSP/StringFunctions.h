#pragma once
#include <string>

using namespace std;

class StringFunctions
{
public:
	static bool starts_with(string s, string pattern);
	static string trim(string s);
	static string get_substring_after_char(string s, char c);

private:
	StringFunctions() { }
};