#pragma once
#include "Instance.h"

class Algorithms
{
public:
	static void greedy(Instance* instance, int start);
	static void brute_force(Instance* instance);

private:
	static int compute_tour_length(Instance* instance, int* tour);
	static void display_improvement(int tour_length);
	static void display_tour(int tour_length, int* tour, int size);
	static void permute(Instance* instance, int* a, int l, int r, int* best_tour_length, int* best_tour);
};