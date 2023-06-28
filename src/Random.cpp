#include "Random.h"

#include <algorithm>

bool Random::is_initialised = false;
mt19937 Random::mersenne_twister;

void Random::init()
{
    if (!is_initialised)
    {
        is_initialised = true;

		random_device source;
		int* seed = new int[mt19937::state_size];
		generate(&seed[0], &seed[mt19937::state_size], ref(source));
		seed_seq seq(&seed[0], &seed[mt19937::state_size]);
		mt19937 mt(seq);

		delete[] seed;

		mersenne_twister = mt;
    }    
}

int Random::next(int max)
{
	return uniform_int_distribution<int>{0, max}(mersenne_twister);
}

double Random::next_double()
{
	return uniform_real_distribution<double>{0.0, 1.0}(mersenne_twister);
}