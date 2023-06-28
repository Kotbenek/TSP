#include "Algorithms.h"
#include "Random.h"
#include "Individual.h"

int* Algorithms::greedy(Instance* instance, int start, bool display)
{
	int* tour = new int[instance->size];
	tour[0] = start;

	//Unvisited cities array
	int* unvisited = new int[instance->size - 1];
	{
		int index = 0;
		for (int i = 0; i < instance->size; i++)
		{
			if (i == start) continue;
			unvisited[index] = i;
			index++;
		}
	}

	int unvisited_size = instance->size - 1;	
	int current_location = start;

	//While there are cities to visit
	while (unvisited_size > 0)
	{
		//Find the closest city from current location
		int min = instance->edge_weight(current_location, unvisited[0]);
		int closest = 0;

		for (int i = 1; i < unvisited_size; i++)
		{
			if (min > instance->edge_weight(current_location, unvisited[i]) && current_location != unvisited[i])
			{
				min = instance->edge_weight(current_location, unvisited[i]);
				closest = i;
			}
		}

		//Add the closest city to the tour
		tour[instance->size - unvisited_size] = unvisited[closest];

		//Update current location
		current_location = unvisited[closest];

		//Remove visited city from the unvisited cities array
		unvisited_size--;
		swap(unvisited[closest], unvisited[unvisited_size]);
		
	}

	if (display) display_tour(compute_tour_length(instance, tour), instance->optimal_tour_length, tour, instance->size);

	if (display) delete[] tour;
	delete[] unvisited;

	return tour;
}

void Algorithms::repetitive_greedy(Instance* instance)
{
	int* best_tour = greedy(instance, 0, false);
	int64_t best_tour_length = compute_tour_length(instance, best_tour);

	int* tour;
	int64_t tour_length;

	//Repeat the greedy algorithm for each city and choose the best tour
	for (int i = 1; i < instance->size; i++)
	{
		tour = greedy(instance, i, false);
		tour_length = compute_tour_length(instance, tour);

		if (tour_length < best_tour_length)
		{
			copy(&tour[0], &tour[instance->size], best_tour);
			
			best_tour_length = tour_length;
		}

		delete[] tour;
	}

	display_tour(best_tour_length, instance->optimal_tour_length, best_tour, instance->size);

	delete[] best_tour;
}

void Algorithms::brute_force(Instance* instance)
{
	//Prepare permutation array
	int* a = new int[instance->size];
	for (int i = 0; i < instance->size; i++) a[i] = i;

	//Variables for storing the best found tour
	int64_t best_tour_length = INT64_MAX;
	int* best_tour = new int[instance->size];

	permute(instance, a, 1, instance->size, &best_tour_length, best_tour);

	display_tour(best_tour_length, instance->optimal_tour_length, best_tour, instance->size);

	delete[] a;
	delete[] best_tour;
}

void Algorithms::simulated_annealing(Instance* instance, double T_start, double T_end, int epoch_length, double cooling_factor, void (*neighbourhood_type)(int* tour, int i1, int i2))
{
	int* tour = generate_random_tour(instance->size);
	int64_t tour_length = compute_tour_length(instance, tour);
	
	//Temperature
	double T = T_start;

	display_tour_length(tour_length, instance->optimal_tour_length);

	int* new_tour = new int[instance->size];
	int64_t new_tour_length;

	int* best_tour = new int[instance->size];
	copy(&tour[0], &tour[instance->size], best_tour);
	int64_t best_tour_length = tour_length;

	//While the temperature is higher than minimal temperature
	while (T > T_end)
	{
		//Run epoch
		for (int i = 0; i < epoch_length; i++)
		{
			//Get neighbour from tour neighbourhood
			copy(&tour[0], &tour[instance->size], new_tour);			
			neighbourhood_type(new_tour, Random::next(instance->size - 1), Random::next(instance->size - 1));
			new_tour_length = compute_tour_length(instance, new_tour);

			int64_t delta = new_tour_length - tour_length;

			//If new tour is better than current tour
			if (delta < 0)
			{
				//Store new tour
				copy(&new_tour[0], &new_tour[instance->size], tour);
				tour_length = new_tour_length;

				//If new tour is better than the best found tour
				if (new_tour_length < best_tour_length)
				{
					//Store new tour
					copy(&new_tour[0], &new_tour[instance->size], best_tour);
					best_tour_length = new_tour_length;

					display_tour_length(best_tour_length, instance->optimal_tour_length);
				}
			}
			else
			{
				//Accept new tour with acceptance probability
				double p = Random::next_double();

				if (p < exp(-delta / T))
				{
					copy(&new_tour[0], &new_tour[instance->size], tour);
					tour_length = new_tour_length;
				}
			}
		}

		//Apply geometric cooling scheme
		T = T * cooling_factor;
	}

	display_tour(best_tour_length, instance->optimal_tour_length, best_tour, instance->size);

	delete[] tour;
	delete[] new_tour;
	delete[] best_tour;
}

void Algorithms::genetic(Instance* instance, int population_size, int max_iterations_with_no_improvement, double mutation_factor, double crossover_factor, int percentage_of_population_to_crossover)
{
	//Generate population - 5% greedy, 95% random
	Individual** population = new Individual * [population_size];

	int* greedy = Algorithms::greedy(instance, 0, false);
	for (int i = 0; i < 5 * population_size / 100; i++)
	{
		population[i] = new Individual(instance->size, greedy);
	}
	delete[] greedy;

	for (int i = 5 * population_size / 100; i < population_size; i++)
	{
		int* t = Algorithms::generate_random_tour(instance->size);
		population[i] = new Individual(instance->size, t);
		delete[] t;
	}

	Individual* best_individual = Individual::clone(population[0], instance->size);
	int64_t best_individual_tour_length = compute_tour_length(instance, best_individual->tour);

	display_tour_length(best_individual_tour_length, instance->optimal_tour_length);

	int iterations_with_no_improvement = 0;
	int parents_size = percentage_of_population_to_crossover * population_size / 100;
	int tournament_participants_size = parents_size * 8 > population_size ? population_size / parents_size : 8;

	int* population_picker_template = new int[population_size];
	for (int i = 0; i < population_size; i++) population_picker_template[i] = i;

	int population_with_children_size = population_size + 2 * parents_size;

	//Calculate fitness
	for (int i = 0; i < population_size; i++) calculate_fitness(population[i], instance);

	while (true)
	{
		//Stop condition
		if (iterations_with_no_improvement == max_iterations_with_no_improvement) break;

		//Selection
		int* parents = new int[parents_size];
		int* population_picker = new int[population_size];
		int individuals_picked = 0;
		copy(&population_picker_template[0], &population_picker_template[population_size], population_picker);
		random_shuffle_array(population_picker, population_size);
		for (int i = 0; i < parents_size; i++)
		{
			int best = 0;
			double best_value = population[population_picker[i * tournament_participants_size]]->fitness;
			for (int j = 1; j < tournament_participants_size; j++)
			{
				if (population[population_picker[i * tournament_participants_size + j]]->fitness > best_value)
				{
					best = j;
					best_value = population[population_picker[i * tournament_participants_size + j]]->fitness;
				}
			}
			parents[individuals_picked] = population_picker[i * tournament_participants_size + best];
			individuals_picked++;
		}
		
		//Crossover
		Individual** population_with_children = new Individual * [population_with_children_size];

		copy(&population[0], &population[population_size], population_with_children);

		for (int i = 0; i < parents_size; i++)
		{
			double p = Random::next_double();

			Individual** children;

			if (p < crossover_factor)
			{
				children = Individual::crossover_OX(population[parents[i]], population[parents[(i + 1) % parents_size]], instance->size);
				calculate_fitness(children[0], instance);
				calculate_fitness(children[1], instance);
			}
			else
			{
				children = new Individual * [2];
				children[0] = Individual::clone(population[parents[i]], instance->size);
				children[1] = Individual::clone(population[parents[(i + 1) % parents_size]], instance->size);
			}
			population_with_children[population_size + 2 * i] = children[0];
			population_with_children[population_size + 2 * i + 1] = children[1];

			delete[] children;
		}
		delete[] parents;
		delete[] population_picker;

		//Mutation
		for (int i = 0; i < population_with_children_size; i++)
		{
			double p = Random::next_double();
			if (p < mutation_factor)
			{
				if (Random::next(1) == 0)
				{
					neighbour_invert(population_with_children[i]->tour, Random::next(instance->size - 1), Random::next(instance->size - 1));
				}
				else
				{
					neighbour_insert(population_with_children[i]->tour, Random::next(instance->size - 1), Random::next(instance->size - 1));
				}
				calculate_fitness(population_with_children[i], instance);
			}
		}

		//Sort by fitness descending
		Individual::quicksort_desc(population_with_children, 0, population_with_children_size - 1);

		//Make new population
		copy(&population_with_children[0], &population_with_children[population_size], population);

		//Delete unused individuals
		for (int i = population_size; i < population_with_children_size; i++) delete population_with_children[i];
		delete[] population_with_children;

		//Check if better solution is found
		int64_t best_in_population_tour_length = compute_tour_length(instance, population[0]->tour);
		if (best_individual_tour_length > best_in_population_tour_length)
		{
			delete best_individual;

			best_individual = Individual::clone(population[0], instance->size);
			best_individual_tour_length = best_in_population_tour_length;
			iterations_with_no_improvement = 0;
			display_tour_length(best_individual_tour_length, instance->optimal_tour_length);
		}
		else iterations_with_no_improvement++;		
	}

	display_tour(best_individual_tour_length, instance->optimal_tour_length, best_individual->tour, instance->size);

	for (int i = 0; i < population_size; i++) delete population[i];
	delete[] population;

	delete best_individual;
	delete[] population_picker_template;
}

void Algorithms::neighbour_swap(int* tour, int i1, int i2)
{
	swap(tour[i1], tour[i2]);
}

void Algorithms::neighbour_invert(int* tour, int i1, int i2)
{
	if (i1 > i2) swap(i1, i2);

	while (i1 < i2)
	{
		swap(tour[i1], tour[i2]);
		i1++;
		i2--;
	}
}

void Algorithms::neighbour_insert(int* tour, int i1, int i2)
{
	if (i1 > i2) swap(i1, i2);

	int temp = tour[i2];
	copy(&tour[i1], &tour[i2], &tour[i1 + 1]);
	tour[i1] = temp;
}

int64_t Algorithms::compute_tour_length(Instance* instance, int* tour)
{
	int64_t length = 0;

	//Add edge weights (1)-(2), (2)-(3), ..., (n-1)-(n)
	for (int i = 0; i < instance->size - 1; i++)
	{
		length += instance->edge_weight(tour[i], tour[i + 1]);
	}

	//Add edge weight (n)-(0)
	length += instance->edge_weight(tour[instance->size - 1], tour[0]);

	return length;
}

void Algorithms::display_tour_length(int64_t tour_length, int64_t optimal_tour_length)
{
	//Save cout flags
	ios_base::fmtflags f(cout.flags());

	cout << tour_length;
	if (optimal_tour_length > -1) cout << " (" << fixed << setprecision(2) << (100.0 * tour_length / optimal_tour_length) << "%)";
	cout << "\r\n";

	//Restore cout flags
	cout.flags(f);
}

void Algorithms::display_tour(int64_t tour_length, int64_t optimal_tour_length, int* tour, int size)
{
	cout << "Tour: ";
	for (int i = 0; i < size - 1; i++)
	{
		cout << tour[i] + 1 << " ";
	}
	cout << tour[size - 1] + 1 << "\r\n";

	cout << "Tour length: ";
	display_tour_length(tour_length, optimal_tour_length);
	cout << "\r\n";
}

void Algorithms::permute(Instance* instance, int* a, int l, int r, int64_t* best_tour_length, int* best_tour)
{
	if (l == r)
	{
		//Check generated permutation
		int64_t tour_length = compute_tour_length(instance, a);

		//If generated permutation is better than the best found tour
		if (tour_length < *best_tour_length)
		{
			//Store generated permutation
			*best_tour_length = tour_length;
			copy(&a[0], &a[instance->size], best_tour);

			display_tour_length(tour_length, instance->optimal_tour_length);
		}
	}
	else
	{
		//Generate permutations
		for (int i = l; i < r; i++)
		{
			swap(a[l], a[i]);
			permute(instance, a, l + 1, r, best_tour_length, best_tour);
			swap(a[l], a[i]);
		}
	}
}

int* Algorithms::generate_random_tour(int size)
{
	int* tour = new int[size];
	tour[0] = 0;

	int unvisited_size = size - 1;
	int* unvisited = new int[unvisited_size];
	for (int i = 0; i < unvisited_size; i++) unvisited[i] = i + 1;

	while (unvisited_size > 0)
	{
		int index = Random::next(unvisited_size - 1);
		tour[size - unvisited_size] = unvisited[index];

		//Remove visited city from the unvisited cities array
		unvisited_size--;
		swap(unvisited[index], unvisited[unvisited_size]);
	}

	delete[] unvisited;

	return tour;
}

void Algorithms::random_shuffle_array(int* a, int size)
{
	for (int i = size - 1; i > 0; i--) swap(a[i], a[Random::next(i)]);	
}

void Algorithms::calculate_fitness(Individual* individual, Instance* instance)
{
	individual->fitness = 1000000.0 / (double)compute_tour_length(instance, individual->tour);
}

