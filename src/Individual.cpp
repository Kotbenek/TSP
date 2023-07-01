#include "Individual.h"
#include "Random.h"
#include <algorithm>

Individual::Individual(int size, int* tour)
{
	this->tour = new int[size];
	fitness = 0.0;

	if (tour != 0) std::copy(&tour[0], &tour[size], this->tour);
}

Individual::~Individual()
{
	delete[] tour;
}

Individual* Individual::clone(Individual* individual, int size)
{
	Individual* i = new Individual(size, individual->tour);
	i->fitness = individual->fitness;
	return i;
}

Individual** Individual::crossover_OX(Individual* i1, Individual* i2, int size)
{
	Individual** children = new Individual * [2];
	children[0] = new Individual(size);
	children[1] = new Individual(size);

	int k1 = Random::next(size - 1);
	int k2 = Random::next(size - 1);

	if (k1 > k2) std::swap(k1, k2);

	//Inherit the section between k1 and k2
	std::copy(&i2->tour[k1], &i2->tour[k2], &children[0]->tour[k1]);
	std::copy(&i1->tour[k1], &i1->tour[k2], &children[1]->tour[k1]);

	//Available cities (1 - available, 0 - already used)
	int* available_1 = new int[size];
	int* available_2 = new int[size];

	std::fill(&available_1[0], &available_1[size], 1);
	std::fill(&available_2[0], &available_2[size], 1);

	for (int i = k1; i < k2; i++)
	{
		available_1[children[0]->tour[i]] = 0;
		available_2[children[1]->tour[i]] = 0;
	}

	//Complete the tours using parents' data
	int index = k2;
	int new_1_index = k2;
	int new_2_index = k2;

	do
	{
		if (available_1[i1->tour[index]] != 0)
		{
			available_1[i1->tour[index]] = 0;
			children[0]->tour[new_1_index] = i1->tour[index];
			new_1_index++;
			if (new_1_index == size) new_1_index = 0;
		}

		if (available_2[i2->tour[index]] != 0)
		{
			available_2[i2->tour[index]] = 0;
			children[1]->tour[new_2_index] = i2->tour[index];
			new_2_index++;
			if (new_2_index == size) new_2_index = 0;
		}

		index++;
		if (index == size) index = 0;

	} while (index != k2);

	delete[] available_1;
	delete[] available_2;

	return children;
}

void Individual::quicksort_desc(Individual** a, int l, int h)
{
	if (l < h)
	{
		int p = partition(a, l, h);
		quicksort_desc(a, l, p);
		quicksort_desc(a, p + 1, h);
	}
}

int Individual::partition(Individual** a, int l, int h)
{
	double pivot = a[(int)floor((l + h) / 2)]->fitness;
	int i = l - 1;
	int j = h + 1;
	while (true)
	{
		do
		{
			i++;
		} while (a[i]->fitness > pivot);
		do
		{
			j--;
		} while (a[j]->fitness < pivot);

		if (i >= j) return j;
		std::swap(a[i], a[j]);
	}
}