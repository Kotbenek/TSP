#include "Algorithms.h"
#include "Random.h"

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

	if (display) display_tour(compute_tour_length(instance, tour), tour, instance->size);

	if (display) delete[] tour;
	delete[] unvisited;

	return tour;
}

void Algorithms::repetitive_greedy(Instance* instance)
{
	int* best_tour = greedy(instance, 0, false);
	int best_tour_length = compute_tour_length(instance, best_tour);

	int* tour;
	int tour_length;

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

	display_tour(best_tour_length, best_tour, instance->size);

	delete[] best_tour;
}

void Algorithms::brute_force(Instance* instance)
{
	//Prepare permutation array
	int* a = new int[instance->size];
	for (int i = 0; i < instance->size; i++) a[i] = i;

	//Variables for storing the best found tour
	int best_tour_length = INT32_MAX;
	int* best_tour = new int[instance->size];

	permute(instance, a, 1, instance->size, &best_tour_length, best_tour);

	display_tour(best_tour_length, best_tour, instance->size);

	delete[] a;
	delete[] best_tour;
}

void Algorithms::simulated_annealing(Instance* instance, double T_start, double T_end, int epoch_length, double cooling_factor, void (*neighbourhood_type)(int* tour, int i1, int i2))
{
	int* tour = generate_random_tour(instance->size);
	int tour_length = compute_tour_length(instance, tour);
	
	//Temperature
	double T = T_start;

	display_tour_length(tour_length);

	int* new_tour = new int[instance->size];
	int new_tour_length;

	int* best_tour = new int[instance->size];
	copy(&tour[0], &tour[instance->size], best_tour);
	int best_tour_length = tour_length;

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

			int delta = new_tour_length - tour_length;

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

					display_tour_length(best_tour_length);
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

	display_tour(best_tour_length, best_tour, instance->size);

	delete[] tour;
	delete[] new_tour;
	delete[] best_tour;
}

void Algorithms::neighbour_swap(int* tour, int i1, int i2)
{
	swap(tour[i1], tour[i2]);
}

int Algorithms::compute_tour_length(Instance* instance, int* tour)
{
	int length = 0;

	//Add edge weights (1)-(2), (2)-(3), ..., (n-1)-(n)
	for (int i = 0; i < instance->size - 1; i++)
	{
		length += instance->edge_weight(tour[i], tour[i + 1]);
	}

	//Add edge weight (n)-(0)
	length += instance->edge_weight(tour[instance->size - 1], tour[0]);

	return length;
}

void Algorithms::display_tour_length(int tour_length)
{
	cout << tour_length << "\r\n";
}

void Algorithms::display_tour(int tour_length, int* tour, int size)
{
	cout << "Tour: ";
	for (int i = 0; i < size - 1; i++)
	{
		cout << tour[i] + 1 << " ";
	}
	cout << tour[size - 1] + 1 << "\r\n";

	cout << "Tour length: " << tour_length << "\r\n" << "\r\n";
}

void Algorithms::permute(Instance* instance, int* a, int l, int r, int* best_tour_length, int* best_tour)
{
	if (l == r)
	{
		//Check generated permutation
		int tour_length = compute_tour_length(instance, a);

		//If generated permutation is better than the best found tour
		if (tour_length < *best_tour_length)
		{
			//Store generated permutation
			*best_tour_length = tour_length;
			copy(&a[0], &a[instance->size], best_tour);

			display_tour_length(tour_length);
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

	while (unvisited_size > 1)
	{
		int index = Random::next(unvisited_size - 1);
		tour[size - unvisited_size] = unvisited[index];

		//Remove visited city from the unvisited cities array
		unvisited_size--;
		swap(unvisited[index], unvisited[unvisited_size]);
	}
	tour[size - 1] = unvisited[0];

	delete[] unvisited;

	return tour;
}