#pragma once
#include "Instance.h"

class Algorithms
{
public:
	static int* greedy(Instance* instance, int start, bool display);
	static void repetitive_greedy(Instance* instance);
	static void brute_force(Instance* instance);
	static void simulated_annealing(Instance* instance, double T_start, double T_end, int epoch_length, double cooling_factor, void (*neighbourhood_type)(int* tour, int i1, int i2));

	static void neighbour_swap(int* tour, int i1, int i2);
	static void neighbour_invert(int* tour, int i1, int i2);

private:
	static int compute_tour_length(Instance* instance, int* tour);
	static void display_tour_length(int tour_length);
	static void display_tour(int tour_length, int* tour, int size);
	static void permute(Instance* instance, int* a, int l, int r, int* best_tour_length, int* best_tour);
	static int* generate_random_tour(int size);
};