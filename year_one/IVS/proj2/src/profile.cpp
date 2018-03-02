#include "stddeviation.hpp"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char** argv)
{
	if (argc != 2)
		exit(EXIT_FAILURE);

	char* pos = nullptr;
	int number = strtol(argv[1], &pos, 10);
	if (*pos != '\0')
		exit(EXIT_FAILURE);

	if (number <= 0)
		exit(EXIT_FAILURE);

	std::ifstream file("nums");

	if (!file.is_open()) {
		std::cout << "Couldn't open file. " << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<double> input;
	for (size_t i = 0; i < number; i++)
	{
		int n;
		file >> n;
		input.push_back(static_cast<double>(n));
	}
	file.close();

	double ret = stddeviation(input);

	return static_cast<int>(ret);
}
