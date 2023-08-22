#pragma once

class Individual
{
public:
    int* tour;
    double fitness;

    Individual(int size, int* tour = 0);
    ~Individual();

    static Individual* clone(Individual* individual, int size);

    static Individual** crossover_OX(Individual* i1, Individual* i2, int size);

    static void quicksort_desc(Individual** a, int l, int h);

private:
    static int partition(Individual** a, int l, int h);
};
