#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

class Instance
{
public:
	int size = -1;
	int** matrix;

	Instance(string filename);
	~Instance();
	void Display();
};