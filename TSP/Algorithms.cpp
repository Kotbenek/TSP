#include "Algorithms.h"

void Algorithms::brute_force(Instance* instance)
{
	cout << "Brute force" << "\r\n";

	//Prepare permutation array
	int* a = new int[instance->size];
	for (int i = 0; i < instance->size; i++) a[i] = i;

	//Variables for storing the best found result
	int best_tour_length = INT32_MAX;
	int* best_tour = new int[instance->size];

	permute(instance, a, 1, instance->size, &best_tour_length, best_tour);

	display_tour(best_tour_length, best_tour, instance->size);

	delete[] a;
	delete[] best_tour;
}

int Algorithms::compute_tour_length(Instance* instance, int* tour)
{
	int length = 0;

	//Add edge weights (1)-(2), (2)-(3), ..., (n-1)-(n)
	for (int i = 0; i < instance->size - 1; i++)
	{
		length += instance->matrix[tour[i]][tour[i + 1]];
	}

	//Add edge weight (n)-(0)
	length += instance->matrix[tour[instance->size - 1]][tour[0]];

	return length;
}

void Algorithms::display_improvement(int tour_length)
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

	cout << "Tour length: " << tour_length << "\r\n";
}

void Algorithms::permute(Instance* instance, int* a, int l, int r, int* best_tour_length, int* best_tour)
{
	if (l == r)
	{
		//Check generated permutation
		int tour_length = compute_tour_length(instance, a);

		//If generated permutation is better than the best found result
		if (tour_length < *best_tour_length)
		{
			//Store generated permutation
			*best_tour_length = tour_length;
			for (int i = 0; i < instance->size; i++) best_tour[i] = a[i];

			display_improvement(tour_length);
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