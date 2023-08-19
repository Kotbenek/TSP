#include "TSP.h"

#include "Instance.h"
#include "Algorithms.h"
#include "Random.h"

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cout << "\n"
                     "Usage: TSP.exe [-d] [-gr] [-gr2] [-bf] [-sa t1 t2 el cf nt] [-ga ps mi mf cf pc] -f1|-f2 filename\n"
                     "\n"
                     "Options:\n"
                     "    -d         Display the matrix\n"
                     "    -gr        Solve using greedy algorithm\n"
                     "    -gr2       Solve using repetitive greedy algorithm\n"
                     "    -bf        Solve using brute force algorithm\n"
                     "    -sa        Solve using simulated annealing algorithm\n"
                     "       t1         Starting temperature\n"
                     "       t2         Minimal temperature\n"
                     "       el         Epoch length\n"
                     "       cf         Cooling factor (geometric cooling scheme)\n"
                     "       nt         Neighbourhood type\n"
                     "    -ga        Solve using genetic algorithm\n"
                     "       ps         Population size\n"
                     "       mi         Max iterations with no improvement\n"
                     "       mf         Mutation factor\n"
                     "       cf         Crossover factor\n"
                     "       pc         Percentage of population to crossover\n"
                     "\n"

                     "Neighbourhood types:\n"
                     "    swp        Swap\n"
                     "    inv        Invert\n"
                     "    ins        Insert\n"

                     "\n"

                     "File loading:\n"
                     "    -f1        Load the file as matrix (faster, but needs more memory)\n"
                     "    -f2        Load the file as coords (slower, but needs very little memory)(not available for explicit-type instances)\n"
                     "    filename   Instance file in TSPLIB format\n";
    }
    else
    {
        Random::init();

        std::string filename = argv[argc - 1];
        std::string file_loading = argv[argc - 2];
        Instance* instance;
        if (file_loading == "-f1") instance = new Instance(filename, true);
        else if (file_loading == "-f2") instance = new Instance(filename, false);
        else { std::cout << "Bad argument -f1|-f2: \"" << file_loading << "\".\n"; return -1; }
        if (instance->size == -1) return -1;

        for (int i = 1; i < argc - 2; i++)
        {
            std::string arg = argv[i];

            if (arg == "-d")
            {
                instance->display();
            }
            else if (arg == "-gr")
            {
                std::cout << "Greedy\n";
                Algorithms::greedy(instance, 0, true);
            }
            else if (arg == "-gr2")
            {
                std::cout << "Repetitive greedy\n";
                Algorithms::repetitive_greedy(instance);
            }
            else if (arg == "-bf")
            {
                std::cout << "Brute force\n";
                Algorithms::brute_force(instance);
            }
            else if (arg == "-sa")
            {
                std::cout << "Simulated annealing\n";

                if (argc - 3 - i < 5)
                {
                    std::cout << "Not enough arguments for \"-sa\".\n";
                    break;
                }

                std::string t1 = argv[i + 1];
                std::string t2 = argv[i + 2];
                std::string el = argv[i + 3];
                std::string cf = argv[i + 4];
                std::string nt = argv[i + 5];
                i += 5;

                double T_start;
                double T_end;
                int epoch_length;
                double cooling_factor;
                void (*neighbourhood_type)(int* tour, int i1, int i2);

                if (!StringFunctions::to_double(t1, &T_start) || T_start <= 0.0)
                {
                    std::cout << "Bad argument t1: \"" << t1 << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_double(t2, &T_end) || T_end <= 0.0)
                {
                    std::cout << "Bad argument t2: \"" << t2 << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_int(el, &epoch_length) || epoch_length <= 0)
                {
                    std::cout << "Bad argument el: \"" << el << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_double(cf, &cooling_factor) || cooling_factor <= 0.0 || cooling_factor >= 1.0)
                {
                    std::cout << "Bad argument cf: \"" << cf << "\".\n";
                    continue;
                }

                if (nt == "swp") neighbourhood_type = &Algorithms::neighbour_swap;
                else if (nt == "inv") neighbourhood_type = &Algorithms::neighbour_invert;
                else if (nt == "ins") neighbourhood_type = &Algorithms::neighbour_insert;
                else
                {
                    std::cout << "Bad neighbourhood type \"" << nt << "\".\n";
                    continue;
                }

                std::cout << "   t1: " << T_start << "\n"
                          << "   t2: " << T_end << "\n"
                          << "   el: " << epoch_length << "\n"
                          << "   cf: " << cooling_factor << "\n"
                          << "   nt: " << nt << "\n"
                          << "\n";

                Algorithms::simulated_annealing(instance, T_start, T_end, epoch_length, cooling_factor, neighbourhood_type);
            }
            else if (arg == "-ga")
            {
                std::cout << "Genetic algorithm\n";

                if (argc - 3 - i < 5)
                {
                    std::cout << "Not enough arguments for \"-ga\".\n";
                    break;
                }

                std::string ps = argv[i + 1];
                std::string mi = argv[i + 2];
                std::string mf = argv[i + 3];
                std::string cf = argv[i + 4];
                std::string pc = argv[i + 5];
                i += 5;

                int population_size;
                int max_iterations_with_no_improvement;
                double mutation_factor;
                double crossover_factor;
                int percentage_of_population_to_crossover;

                if (!StringFunctions::to_int(ps, &population_size) || population_size <= 0)
                {
                    std::cout << "Bad argument ps: \"" << ps << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_int(mi, &max_iterations_with_no_improvement) || max_iterations_with_no_improvement <= 0)
                {
                    std::cout << "Bad argument mi: \"" << mi << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_double(mf, &mutation_factor) || mutation_factor <= 0 || mutation_factor >= 1)
                {
                    std::cout << "Bad argument mf: \"" << mf << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_double(cf, &crossover_factor) || crossover_factor <= 0 || crossover_factor >= 1)
                {
                    std::cout << "Bad argument cf: \"" << cf << "\".\n";
                    continue;
                }

                if (!StringFunctions::to_int(pc, &percentage_of_population_to_crossover) || percentage_of_population_to_crossover < 1 || percentage_of_population_to_crossover > 100)
                {
                    std::cout << "Bad argument pc: \"" << pc << "\".\n";
                    continue;
                }

                std::cout << "   ps: " << population_size << "\n"
                          << "   mi: " << max_iterations_with_no_improvement << "\n"
                          << "   mf: " << mutation_factor << "\n"
                          << "   cf: " << crossover_factor << "\n"
                          << "   pc: " << percentage_of_population_to_crossover << "\n"
                          << "\n";

                Algorithms::genetic(instance, population_size, max_iterations_with_no_improvement, mutation_factor, crossover_factor, percentage_of_population_to_crossover);
            }
            else
            {
                std::cout << "Incorrect argument \"" << arg << "\"\n";
            }
        }

        delete instance;
    }
}