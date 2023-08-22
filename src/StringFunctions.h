#pragma once
#include <string>

class StringFunctions
{
public:
    static bool starts_with(std::string s, std::string pattern);
    static std::string trim(std::string s);
    static std::string get_substring_after_char(std::string s, char c);
    static bool to_int(std::string input, int* output);
    static bool to_int64(std::string input, int64_t* output);
    static bool to_double(std::string input, double* output);

private:
    StringFunctions() {}

    template<typename T>
    static bool convert(std::string input, T* output);
};
