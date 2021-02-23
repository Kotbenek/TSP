#include "Instance.h"
#include "Algorithms.h"
#include "Random.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "\r\n";
        cout << "Usage: TSP.exe [-d] [-gr] [-gr2] [-bf] [-sa T1 T2 el cf nt] filename" << "\r\n";

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
        cout << "\r\n";
        cout << "Neighbourhood types:" << "\r\n";
        cout << "    swp" << "        " << "Swap" << "\r\n";
    }
    else
    {
        Random::init();

        string filename = argv[argc - 1];
        Instance* instance = new Instance(filename);
        if (instance->size == -1) return -1;

        for (int i = 1; i < argc - 1; i++)
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

                if (argc - 2 - i < 5)
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

                if (!StringFunctions::to_double(t1, &T_start))
                {
                    cout << "Bad argument t1: \"" << t1 << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_double(t2, &T_end))
                {
                    cout << "Bad argument t2: \"" << t2 << "\"." << "\r\n";
                    continue;
                }
                
                if (!StringFunctions::to_int(el, &epoch_length))
                {
                    cout << "Bad argument el: \"" << el << "\"." << "\r\n";
                    continue;
                }

                if (!StringFunctions::to_double(cf, &cooling_factor))
                {
                    cout << "Bad argument cf: \"" << cf << "\"." << "\r\n";
                    continue;
                }

                if (nt == "swp")
                {
                    neighbourhood_type = &Algorithms::neighbour_swap;
                }
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
            else
            {
                cout << "Incorrect argument \"" << arg << "\"" << "\r\n";
            }
        }

        delete instance;

        system("pause");
    }
}