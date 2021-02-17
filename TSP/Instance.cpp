#include "Instance.h"

Instance::Instance(string filename)
{
	//Specification variables
	string name;
	string comment;
	string type;
	string dimension;
	string edge_weight_type;
	string edge_weight_format;
	int data_start = 0;

	//Read the file
	ifstream f(filename);
	if (f.fail()) { cout << "Something went wrong while loading the file." << "\r\n"; return; }
	string line;
	vector<string> file;

	while (getline(f, line))
	{
		if (StringFunctions::starts_with(line, "NAME"))
		{
			name = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
		}
		else if (StringFunctions::starts_with(line, "COMMENT"))
		{
			comment = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
		}
		else if (StringFunctions::starts_with(line, "TYPE"))
		{
			type = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
		}
		else if (StringFunctions::starts_with(line, "DIMENSION"))
		{
			dimension = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
		}
		else if (StringFunctions::starts_with(line, "EDGE_WEIGHT_TYPE"))
		{
			edge_weight_type = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
		}
		else if (StringFunctions::starts_with(line, "EDGE_WEIGHT_FORMAT"))
		{
			edge_weight_format = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
		}
		else
		{
			stringstream ss(line);
			string s;

			while (getline(ss, s, ' '))
			{
				if (s.length() > 0) file.push_back(s);
			}
		}
	}	

	for (int i = 0; i < file.size(); i++)
	{
		if (file[i] == "NODE_COORD_SECTION" || file[i] == "EDGE_WEIGHT_SECTION") { data_start = i; break; }
	}

	//Store instance size
	stringstream ss(dimension);
	ss >> size;

	//Create matrix
	matrix = new int* [size];
	for (int i = 0; i < size; i++) matrix[i] = new int[size];

	//Display instance information
	cout << "Instance: " << name << "\r\n";
	cout << "Comment: " << comment << "\r\n";
	cout << "Type: " << type << "\r\n";
	cout << "Size: " << size << "\r\n";
	cout << "\r\n";

	//Parse data
	if (edge_weight_type == "EXPLICIT")
	{
		//Move to the data section
		int i = data_start;
		while (file[i] != "EDGE_WEIGHT_SECTION") i++;
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
		else if (edge_weight_format == "UPPER_ROW" || edge_weight_format == "LOWER_COL")
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
		else if (edge_weight_format == "LOWER_ROW" || edge_weight_format == "UPPER_COL")
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
		else if (edge_weight_format == "UPPER_DIAG_ROW" || edge_weight_format == "LOWER_DIAG_COL")
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
		else if (edge_weight_format == "LOWER_DIAG_ROW" || edge_weight_format == "UPPER_DIAG_COL")
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
	}
	else
	{
		//Move to the data section
		int i = data_start;
		while (file[i] != "NODE_COORD_SECTION") i++;
		i++;

		if (edge_weight_type == "EUC_2D")
		{
			double** coords = parse_coords(file, i, 2);

			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < size; k++)
				{
					if (j == k) { matrix[j][k] = -1; continue; }

					double xd = coords[j][0] - coords[k][0];
					double yd = coords[j][1] - coords[k][1];

					matrix[j][k] = nint(sqrt(xd * xd + yd * yd));
				}
			}

			for (int i = 0; i < size; i++) delete[] coords[i];
			delete[] coords;
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

int Instance::nint(double d)
{
	return (int)(d + 0.5);
}

double** Instance::parse_coords(vector<string> file, int i, int dimensions)
{
	//Create 2D array for coords
	double** coords = new double* [size];
	for (int j = 0; j < size; j++) coords[j] = new double[dimensions];

	//Parse coords
	for (int j = 0; j < size; j++)
	{
		int index;
		stringstream ss(file[i]);
		ss >> index;
		index--;
		i++;
		for (int k = 0; k < dimensions; k++)
		{
			stringstream ss(file[i]);
			ss >> coords[index][k];
			i++;
		}
	}

	return coords;
}