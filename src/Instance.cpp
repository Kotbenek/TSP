#include "Instance.h"

#include <cmath>

Instance::Instance(std::string filename, bool parse_data_to_matrix)
{
	//Specification variables
	std::string name;
	std::string comment;
	std::string type;
	std::string dimension;
	std::string edge_weight_type;
	std::string edge_weight_format;
	int data_start = 0;

	//Read the file
	std::ifstream f(filename);
	if (f.fail()) { std::cout << "Something went wrong while loading the file." << "\r\n"; return; }
	std::string line;
	std::vector<std::string> file;

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
			std::stringstream ss(line);
			std::string s;

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
	StringFunctions::to_int(dimension, &size);

	if (size < 0) { std::cout << "Bad instance size. File is corrupted and/or not supported." << "\r\n"; return; }

	//Store the information if instance data is stored as matrix or as coords
	is_instance_data_in_matrix = parse_data_to_matrix;

	if (is_instance_data_in_matrix)
	{
		//Create matrix
		matrix = new int* [size];
		for (int i = 0; i < size; i++) matrix[i] = new int[size];
	}

	//Display instance information
	std::cout << "Instance: " << name << "\r\n";
	std::cout << "Comment: " << comment << "\r\n";
	std::cout << "Type: " << type << "\r\n";
	std::cout << "Size: " << size << "\r\n";
	std::cout << "\r\n";

	//Parse data
	if (edge_weight_type == "EXPLICIT")
	{
		//Storing explicit-type instances as coords is not possible
		if (!is_instance_data_in_matrix) { std::cout << "Explicit-type instances cannot be stored as coords." << "\r\n"; size = -1; return; }

		//Move to the data section
		int i = data_start;
		while (i < file.size() && file[i] != "EDGE_WEIGHT_SECTION") i++;
		i++;

		if (edge_weight_format == "FULL_MATRIX")
		{
			for (int j = 0; j < size; j++)
			{
				for (int k = 0; k < size; k++)
				{
					StringFunctions::to_int(file[i], &matrix[j][k]);
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
					StringFunctions::to_int(file[i], &matrix[j][k]);
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
					StringFunctions::to_int(file[i], &matrix[j][k]);
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
					StringFunctions::to_int(file[i], &matrix[j][k]);
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
					StringFunctions::to_int(file[i], &matrix[j][k]);
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
		while (i < file.size() && file[i] != "NODE_COORD_SECTION") i++;
		i++;

		if (edge_weight_type == "EUC_2D")
		{
			coords = parse_coords(file, i, 2);
			coords_weight_type = CoordsWeightType::EUC_2D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = euc_2d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "EUC_3D")
		{
			coords = parse_coords(file, i, 3);
			coords_weight_type = CoordsWeightType::EUC_3D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = euc_3d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "MAX_2D")
		{
			coords = parse_coords(file, i, 2);
			coords_weight_type = CoordsWeightType::MAX_2D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = max_2d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "MAX_3D")
		{
			coords = parse_coords(file, i, 3);
			coords_weight_type = CoordsWeightType::MAX_3D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = max_3d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "MAN_2D")
		{
			coords = parse_coords(file, i, 2);
			coords_weight_type = CoordsWeightType::MAN_2D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = man_2d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "MAN_3D")
		{
			coords = parse_coords(file, i, 3);
			coords_weight_type = CoordsWeightType::MAN_3D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = man_3d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "CEIL_2D")
		{
			coords = parse_coords(file, i, 2);
			coords_weight_type = CoordsWeightType::CEIL_2D;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = ceil_2d_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else if (edge_weight_type == "GEO")
		{
			coords = parse_coords(file, i, 2);
			coords_weight_type = CoordsWeightType::GEO;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = geo_distance(j, k);
					}
				}
				delete[] coords;
			}
		}
		else if (edge_weight_type == "ATT")
		{
			coords = parse_coords(file, i, 2);
			coords_weight_type = CoordsWeightType::ATT;

			if (is_instance_data_in_matrix)
			{
				for (int j = 0; j < size; j++)
				{
					for (int k = 0; k < size; k++)
					{
						matrix[j][k] = att_distance(j, k);
					}
				}

				for (int i = 0; i < size; i++) delete[] coords[i];
				delete[] coords;
			}
		}
		else
		{
			std::cout << "Edge weight type \"" << edge_weight_type << "\" is not supported." << "\r\n";
			coords_weight_type = CoordsWeightType::COORDS_UNDEFINED;
		}
	}

	load_optimal_tour_length(filename + ".opt");
}

Instance::~Instance()
{
	if (size > 0)
	{
		if (is_instance_data_in_matrix)
		{
			for (int i = 0; i < size; i++) delete[] matrix[i];
			delete[] matrix;
		}
		else
		{
			for (int i = 0; i < size; i++) delete[] coords[i];
			delete[] coords;
		}
	}
}

int Instance::edge_weight(int from, int to)
{
	if (is_instance_data_in_matrix)	return matrix[from][to];

	switch (coords_weight_type)
	{
		case CoordsWeightType::EUC_2D: { return euc_2d_distance(from, to); }
		case CoordsWeightType::EUC_3D: { return euc_3d_distance(from, to); }
		case CoordsWeightType::MAX_2D: { return max_2d_distance(from, to); }
		case CoordsWeightType::MAX_3D: { return max_3d_distance(from, to); }
		case CoordsWeightType::MAN_2D: { return man_2d_distance(from, to); }
		case CoordsWeightType::MAN_3D: { return man_3d_distance(from, to); }
		case CoordsWeightType::CEIL_2D: { return ceil_2d_distance(from, to); }
		case CoordsWeightType::GEO: { return geo_distance(from, to); }
		case CoordsWeightType::ATT: { return att_distance(from, to); }
		case CoordsWeightType::COORDS_UNDEFINED: { return -1; }
		default: return -1;
	}
}

void Instance::display()
{
	//Save cout flags
	std::ios_base::fmtflags f(std::cout.flags());

	std::cout.fill(' ');

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			std::cout << std::setw(6) << edge_weight(i, j) << " ";
		}
		std::cout << "\r\n";
	}
	std::cout << "\r\n";

	//Restore cout flags
	std::cout.flags(f);
}

int Instance::nint(double d)
{
	return (int)(d + 0.5);
}

double** Instance::parse_coords(std::vector<std::string> file, int i, int dimensions)
{
	//Create 2D array for coords
	double** coords = new double* [size];
	for (int j = 0; j < size; j++) coords[j] = new double[dimensions];

	//Parse coords
	for (int j = 0; j < size; j++)
	{
		int index;
		StringFunctions::to_int(file[i], &index);
		index--;
		i++;
		for (int k = 0; k < dimensions; k++)
		{
			StringFunctions::to_double(file[i], &coords[index][k]);
			i++;
		}
	}

	return coords;
}

int Instance::max(int i1, int i2)
{
	return i1 > i2 ? i1 : i2;
}

int Instance::max(int i1, int i2, int i3)
{
	return i1 > i2 ? i1 > i3 ? i1 : i2 : i2 > i3 ? i2 : i3;
}

void Instance::load_optimal_tour_length(std::string filename)
{
	//Check if the file with optimal path length exists
	//If true, read the file and store the value
	std::ifstream f(filename);
	if (f.fail()) return;

	std::string line;
	getline(f, line);
	StringFunctions::to_int64(line, &optimal_tour_length);
}

int Instance::euc_2d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = coords[from_city][0] - coords[to_city][0];
	double yd = coords[from_city][1] - coords[to_city][1];

	return nint(sqrt(xd * xd + yd * yd));
}

int Instance::euc_3d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = coords[from_city][0] - coords[to_city][0];
	double yd = coords[from_city][1] - coords[to_city][1];
	double zd = coords[from_city][2] - coords[to_city][2];

	return nint(sqrt(xd * xd + yd * yd + zd * zd));
}

int Instance::max_2d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = abs(coords[from_city][0] - coords[to_city][0]);
	double yd = abs(coords[from_city][1] - coords[to_city][1]);

	return max(nint(xd), nint(yd));
}

int Instance::max_3d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = abs(coords[from_city][0] - coords[to_city][0]);
	double yd = abs(coords[from_city][1] - coords[to_city][1]);
	double zd = abs(coords[from_city][2] - coords[to_city][2]);

	return max(nint(xd), nint(yd), nint(zd));
}

int Instance::man_2d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = abs(coords[from_city][0] - coords[to_city][0]);
	double yd = abs(coords[from_city][1] - coords[to_city][1]);

	return nint(xd + yd);
}

int Instance::man_3d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = abs(coords[from_city][0] - coords[to_city][0]);
	double yd = abs(coords[from_city][1] - coords[to_city][1]);
	double zd = abs(coords[from_city][2] - coords[to_city][2]);

	return nint(xd + yd + zd);
}

int Instance::ceil_2d_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = coords[from_city][0] - coords[to_city][0];
	double yd = coords[from_city][1] - coords[to_city][1];

	return (int)ceil(sqrt(xd * xd + yd * yd));
}

int Instance::geo_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double latitude_from;
	double latitude_to;
	double longitude_from;
	double longitude_to;

	double PI = 3.141592;
	double RRR = 6378.388;

	//Convert to radians
	double deg;
	double min;

	deg = (int)(coords[from_city][0]);
	min = coords[from_city][0] - deg;
	latitude_from = PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = (int)(coords[from_city][1]);
	min = coords[from_city][1] - deg;
	longitude_from = PI * (deg + 5.0 * min / 3.0) / 180.0;

	deg = (int)(coords[to_city][0]);
	min = coords[to_city][0] - deg;
	latitude_to = PI * (deg + 5.0 * min / 3.0) / 180.0;
	deg = (int)(coords[to_city][1]);
	min = coords[to_city][1] - deg;
	longitude_to = PI * (deg + 5.0 * min / 3.0) / 180.0;

	//Calculate distance
	double q1 = cos(longitude_from - longitude_to);
	double q2 = cos(latitude_from - latitude_to);
	double q3 = cos(latitude_from + latitude_to);

	return (int)(RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
}

int Instance::att_distance(int from_city, int to_city)
{
	if (from_city == to_city) return -1;

	double xd = coords[from_city][0] - coords[to_city][0];
	double yd = coords[from_city][1] - coords[to_city][1];
	double r = sqrt((xd * xd + yd * yd) / 10.0);
	int t = nint(r);

	return t < r ? t + 1 : t;
}