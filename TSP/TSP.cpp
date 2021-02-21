#include "Instance.h"
#include "Algorithms.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "\r\n";
        cout << "Usage: TSP.exe [-d] [-bf] filename" << "\r\n";

        cout << "\r\n";

        cout << "Options:" << "\r\n";
        cout << "    -d" << "         " << "Display the matrix" << "\r\n";
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
                instance->Display();
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