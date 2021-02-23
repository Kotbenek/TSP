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
        cout << "    sw" << "         " << "Swap" << "\r\n";
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
                Algorithms::greedy(instance, 0, true);
            }
            else if (arg == "-gr2")
            {
                Algorithms::repetitive_greedy(instance);
            }
            else if (arg == "-bf")
            {
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

                stringstream ss;

                stringstream().swap(ss);
                ss << t1;
                ss >> T_start;
                if (ss.fail())
                {
                    ss.clear();
                    ss >> t1;
                    cout << "Bad argument \"t1\"" << "\r\n";
                    continue;
                }
                
                stringstream().swap(ss);
                ss << t2;
                ss >> T_end;
                if (ss.fail())
                {
                    ss.clear();
                    ss >> t2;
                    cout << "Bad argument \"t2\"" << "\r\n";
                    continue;
                }

                stringstream().swap(ss);
                ss << el;
                ss >> epoch_length;
                if (ss.fail())
                {
                    ss.clear();
                    ss >> el;
                    cout << "Bad argument \"el\"" << "\r\n";
                    continue;
                }

                stringstream().swap(ss);
                ss << cf;
                ss >> cooling_factor;
                if (ss.fail())
                {
                    ss.clear();
                    ss >> cf;
                    cout << "Bad argument \"cf\"" << "\r\n";
                    continue;
                }

                if (nt == "sw")
                {
                    neighbourhood_type = &Algorithms::neighbour_swap;
                }
                else
                {
                    cout << "Bad neighbourhood type \"" << nt << "\"." << "\r\n";
                    continue;
                }

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