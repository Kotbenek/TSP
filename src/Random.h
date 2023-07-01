#pragma once
#include <random>

class Random
{
public:
	static void init();
	static int next(int max);
	static double next_double();

private:
	static std::mt19937 mersenne_twister;
	static bool is_initialised;
};