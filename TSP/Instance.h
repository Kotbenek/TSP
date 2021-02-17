#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "StringFunctions.h"

using namespace std;

class Instance
{
public:
	int size = -1;
	int** matrix;

	Instance(string filename);
	~Instance();
	void Display();

private:
	int nint(double d);
	double** parse_coords(vector<string> file, int i, int dimensions);
	int max(int i1, int i2);
	int max(int i1, int i2, int i3);
};