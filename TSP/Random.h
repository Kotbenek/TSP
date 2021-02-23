#pragma once
#include <random>

using namespace std;

class Random
{
public:
	static void init();
	static int next(int max);
	static double next_double();

private:
	static mt19937 mersenne_twister;
	static bool is_initialised;
};