#include "Instance.h"
#include "Algorithms.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "\r\n";
        cout << "Usage: TSP.exe [-d] [-gr] [-gr2] [-bf] filename" << "\r\n";

        cout << "\r\n";

        cout << "Options:" << "\r\n";
        cout << "    -d" << "         " << "Display the matrix" << "\r\n";
        cout << "    -gr" << "        " << "Solve using greedy algorithm" << "\r\n";
        cout << "    -gr2" << "       " << "Solve using repetitive greedy algorithm" << "\r\n";
        cout << "    -bf" << "        " << "Solve using brute force algorithm" << "\r\n";
    }
    else
    {
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
            else
            {
                cout << "Incorrect argument \"" << arg << "\"" << "\r\n";
            }
        }

        delete instance;

        system("pause");
    }
}