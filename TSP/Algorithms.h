#pragma once
#include "Instance.h"
#include "Individual.h"

class Algorithms
{
public:
	static int* greedy(Instance* instance, int start, bool display);
	static void repetitive_greedy(Instance* instance);
	static void brute_force(Instance* instance);
	static void simulated_annealing(Instance* instance, double T_start, double T_end, int epoch_length, double cooling_factor, void (*neighbourhood_type)(int* tour, int i1, int i2));
	static void genetic(Instance* instance, int population_size, int max_iterations_with_no_improvement, double mutation_factor, double crossover_factor, int percentage_of_population_to_crossover);

	static void neighbour_swap(int* tour, int i1, int i2);
	static void neighbour_invert(int* tour, int i1, int i2);
	static void neighbour_insert(int* tour, int i1, int i2);

private:
	static int64_t compute_tour_length(Instance* instance, int* tour);
	static void display_tour_length(int64_t tour_length, int64_t optimal_tour_length);
	static void display_tour(int64_t tour_length, int64_t optimal_tour_length, int* tour, int size);
	static void permute(Instance* instance, int* a, int l, int r, int64_t* best_tour_length, int* best_tour);
	static int* generate_random_tour(int size);
	static void random_shuffle_array(int* a, int size);
	static void calculate_fitness(Individual* individual, Instance* instance);
};