#include "Instance.h"

Instance::Instance(string filename)
{
	//Read the file
	ifstream f(filename);
	if (f.fail()) { cout << "Something went wrong while loading the file." << "\r\n"; return; }
	string line;
	vector<string> file;

	do
	{
		getline(f, line);

		stringstream ss(line);
		string s;

		while (getline(ss, s, ' '))
		{
			if (s.length() > 0) file.push_back(s);				
		}
	} while (line.length() > 0);

	//Extract specification
	string name;
	string dimension;
	string edge_weight_type;
	string edge_weight_format;
	int data_start = 0;

	for (int i = 0; i < file.size(); i++)
	{
		if (file[i] == "NAME:") { name = file[i + 1]; i++; }
		else if (file[i] == "DIMENSION:") { dimension = file[i + 1]; i++; }
		else if (file[i] == "EDGE_WEIGHT_TYPE:") { edge_weight_type = file[i + 1]; i++; }
		else if (file[i] == "EDGE_WEIGHT_FORMAT:") { edge_weight_format = file[i + 1]; i++; }

		else if (file[i] == "NODE_COORD_SECTION" || file[i] == "EDGE_WEIGHT_SECTION") { data_start = i; break; }
	}

	//Store instance size
	stringstream ss(dimension);
	ss >> size;

	//Create matrix
	matrix = new int* [size];
	for (int i = 0; i < size; i++) matrix[i] = new int[size];

	//Display instance information
	cout << "Instance: " << name << "\r\n";
	cout << "Size: " << size << "\r\n";

	//Parse data
	if (edge_weight_type == "EXPLICIT")
	{
		for (int i = data_start; i < file.size(); i++)
		{
			if (file[i] == "EDGE_WEIGHT_SECTION")
			{
				i++;

				if (edge_weight_format == "FULL_MATRIX")
				{
					for (int j = 0; j < size; j++)
					{
						for (int k = 0; k < size; k++)
						{
							stringstream ss(file[i]);
							ss >> matrix[j][k];
							i++;
						}
					}
				}
				else if (edge_weight_format == "UPPER_ROW" || edge_weight_format == "UPPER_COL")
				{
					for (int j = 0; j < size - 1; j++)
					{
						for (int k = j + 1; k < size; k++)
						{
							stringstream ss(file[i]);
							ss >> matrix[j][k];
							matrix[k][j] = matrix[j][k];
							i++;
						}
					}
					for (int j = 0; j < size; j++) matrix[j][j] = -1;
				}
				else if (edge_weight_format == "LOWER_ROW" || edge_weight_format == "LOWER_COL")
				{
					for (int j = 1; j < size; j++)
					{
						for (int k = 0; k < j; k++)
						{
							stringstream ss(file[i]);
							ss >> matrix[j][k];
							matrix[k][j] = matrix[j][k];
							i++;
						}
					}
					for (int j = 0; j < size; j++) matrix[j][j] = -1;
				}
				else if (edge_weight_format == "UPPER_DIAG_ROW" || edge_weight_format == "UPPER_DIAG_COL")
				{					
					for (int j = 0; j < size; j++)
					{
						for (int k = j; k < size; k++)
						{
							stringstream ss(file[i]);
							ss >> matrix[j][k];
							matrix[k][j] = matrix[j][k];
							i++;
						}
					}
				}
				else if (edge_weight_format == "LOWER_DIAG_ROW" || edge_weight_format == "LOWER_DIAG_COL")
				{
					for (int j = 0; j < size; j++)
					{
						for (int k = 0; k < j + 1; k++)
						{
							stringstream ss(file[i]);
							ss >> matrix[j][k];
							matrix[k][j] = matrix[j][k];
							i++;
						}
					}
				}
				break;
			}
		}		
	}
	else if (edge_weight_type == "EUC_2D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "EUC_3D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "MAX_2D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "MAX_3D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "MAN_2D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "MAN_3D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "CEIL_2D")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "GEO")
	{
		throw new exception("not implemented");
	}
	else if (edge_weight_type == "ATT")
	{
		throw new exception("not implemented");
	}
	else
	{
		throw new exception("not implemented");
	}
}

Instance::~Instance()
{
	if (size > 0)
	{
		for (int i = 0; i < size; i++) delete[] matrix[i];
		delete[] matrix;
	}
}

void Instance::Display()
{
	cout.fill(' ');

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << setw(6) << matrix[i][j] << " ";
		}
		cout << "\r\n";
	}
}