#include "Instance.h"
#include "Algorithms.h"
#include "Random.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        cout << "\r\n";
        cout << "Usage: TSP.exe [-d] [-gr] [-gr2] [-bf] [-sa t1 t2 el cf nt] [-ga ps mi mf cf pc] -f1|-f2 filename" << "\r\n";

        cout << "\r\n";

        cout << "Options:" << "\r\n";
        cout << "    -d" << "         " << "Display the matrix" << "\r\n";
        cout << "    -gr" << "        " << "Solve using greedy algorithm" << "\r\n";
        cout << "    -gr2" << "       " << "Solve using repetitive greedy algorithm" << "\r\n";
        cout << "    -bf" << "        " << "Solve using brute force algorithm" << "\r\n";
        cout << "    -sa" << "        " << "Solve using simulated annealing algorithm" << "\r\n";
        cout << "       t1" << "         " << "Starting temperature" << "\r\n";
        cout << "       t2" << "         " << "Minimal temperature" << "\r\n";
        cout << "       el" << "         " << "Epoch length" << "\r\n";
        cout << "       cf" << "         " << "Cooling factor (geometric cooling scheme)" << "\r\n";
        cout << "       nt" << "         " << "Neighbourhood type" << "\r\n";
        cout << "    -ga" << "        " << "Solve using genetic algorithm" << "\r\n";
        cout << "       ps" << "         " << "Population size" << "\r\n";
        cout << "       mi" << "         " << "Max iterations with no improvement" << "\r\n";
        cout << "       mf" << "         " << "Mutation factor" << "\r\n";
        cout << "       cf" << "         " << "Crossover factor" << "\r\n";
        cout << "       pc" << "         " << "Percentage of population to crossover" << "\r\n";

        cout << "\r\n";

        cout << "Neighbourhood types:" << "\r\n";
        cout << "    swp" << "        " << "Swap" << "\r\n";
        cout << "    inv" << "        " << "Invert" << "\r\n";
        cout << "    ins" << "        " << "Insert" << "\r\n";

        cout << "\r\n";

        cout << "File loading:" << "\r\n";
        cout << "    -f1" << "        " << "Load the file as matrix (faster, but needs more memory)" << "\r\n";
        cout << "    -f2" << "        " << "Load the file as coords (slower, but needs very little memory)(not available for explicit-type instances)" << "\r\n";
        cout << "    filename" << "   " << "Instance file in TSPLIB format" << "\r\n";
    }
    else
    {
        Random::init();

        string filename = argv[argc - 1];
        string file_loading = argv[argc - 2];
        Instance* instance;
        if (file_loading == "-f1") instance = new Instance(filename, true);
        else if (file_loading == "-f2") instance = new Instance(filename, false);
        else { cout << "Bad argument -f1|-f2: \"" << file_loading << "\"." << "\r\n"; return -1; }
        if (instance->size == -1) return -1;

        for (int i = 1; i < argc - 2; i++)
        {
            string arg = argv[i];

            if (arg == "-d")
            {
                instance->display();
            }
            else if (arg == "-gr")
            {
                cout << "Greedy" << "\r\n";
                Algorithms::greedy(instance, 0, true);
            }
            else if (arg == "-gr2")
            {
                cout << "Repetitive greedy" << "\r\n";
                Algorithms::repetitive_greedy(instance);
            }
            else if (arg == "-bf")
            {
                cout << "Brute force" << "\r\n";
                Algorithms::brute_force(instance);
            }
            else if (arg == "-sa")
            {
                cout << "Simulated annealing" << "\r\n";

                if (argc - 3 - i < 5)
                {
                    cout << "Not enough arguments for \"-sa\"." << "\r\n";
                    break;
                }

                string t1 = argv[i + 1];
                string t2 = argv[i + 2];
                string el = argv[i + 3];
                string cf = argv[i + 4];
                string nt = argv[i + 5];
                i += 5;

                double T_start;
                double T_end;
                int epoch_length;
                double cooling_factor;
                void (*neighbourhood_type)(int* tour, int i1, int i2);

                if (!StringFunctions::to_double(t1, &T_start) || T_start <= 0.0)
                {
                    cout << "Bad argument t1: \"" << t1 << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_double(t2, &T_end) || T_end <= 0.0)
                {
                    cout << "Bad argument t2: \"" << t2 << "\"." << "\r\n";
                    continue;
                }
                
                if (!StringFunctions::to_int(el, &epoch_length) || epoch_length <= 0)
                {
                    cout << "Bad argument el: \"" << el << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_double(cf, &cooling_factor) || cooling_factor <= 0.0 || cooling_factor >= 1.0)
                {
                    cout << "Bad argument cf: \"" << cf << "\"." << "\r\n";
                    continue;
                }

                if (nt == "swp") neighbourhood_type = &Algorithms::neighbour_swap;
                else if (nt == "inv") neighbourhood_type = &Algorithms::neighbour_invert;
                else if (nt == "ins") neighbourhood_type = &Algorithms::neighbour_insert;
                else
                {
                    cout << "Bad neighbourhood type \"" << nt << "\"." << "\r\n";
                    continue;
                }

                cout << "   t1: " << T_start << "\r\n";
                cout << "   t2: " << T_end << "\r\n";
                cout << "   el: " << epoch_length << "\r\n";
                cout << "   cf: " << cooling_factor << "\r\n";
                cout << "   nt: " << nt << "\r\n";
                cout << "\r\n";

                Algorithms::simulated_annealing(instance, T_start, T_end, epoch_length, cooling_factor, neighbourhood_type);                
            }
            else if (arg == "-ga")
            {
                cout << "Genetic algorithm" << "\r\n";

                if (argc - 3 - i < 5)
                {
                    cout << "Not enough arguments for \"-ga\"." << "\r\n";
                    break;
                }

                string ps = argv[i + 1];
                string mi = argv[i + 2];
                string mf = argv[i + 3];
                string cf = argv[i + 4];
                string pc = argv[i + 5];
                i += 5;

                int population_size;
                int max_iterations_with_no_improvement;
                double mutation_factor;
                double crossover_factor;
                int percentage_of_population_to_crossover;

                if (!StringFunctions::to_int(ps, &population_size) || population_size <= 0)
                {
                    cout << "Bad argument ps: \"" << ps << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_int(mi, &max_iterations_with_no_improvement) || max_iterations_with_no_improvement <= 0)
                {
                    cout << "Bad argument mi: \"" << mi << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_double(mf, &mutation_factor) || mutation_factor <= 0 || mutation_factor >= 1)
                {
                    cout << "Bad argument mf: \"" << mf << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_double(cf, &crossover_factor) || crossover_factor <= 0 || crossover_factor >= 1)
                {
                    cout << "Bad argument cf: \"" << cf << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_int(pc, &percentage_of_population_to_crossover) || percentage_of_population_to_crossover < 1 || percentage_of_population_to_crossover > 100)
                {
                    cout << "Bad argument pc: \"" << pc << "\"." << "\r\n";
                    continue;
                }

                cout << "   ps: " << population_size << "\r\n";
                cout << "   mi: " << max_iterations_with_no_improvement << "\r\n";
                cout << "   mf: " << mutation_factor << "\r\n";
                cout << "   cf: " << crossover_factor << "\r\n";
                cout << "   pc: " << percentage_of_population_to_crossover << "\r\n";
                cout << "\r\n";

                Algorithms::genetic(instance, population_size, max_iterations_with_no_improvement, mutation_factor, crossover_factor, percentage_of_population_to_crossover);
            }
            else
            {
                cout << "Incorrect argument \"" << arg << "\"" << "\r\n";
            }
        }

        delete instance;

        system("pause");
    }
}