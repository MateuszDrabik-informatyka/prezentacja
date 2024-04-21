module;
#include <vector>
#include <thread>
#include <ranges>
#include <fstream>
#include <algorithm>
#include <regex>
#include <string>
#include <iostream>
export module zadanie6;

static void get_matrix(std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& b, std::ifstream& file)
{
	std::regex reg("\\d+");
	std::smatch match;
	std::string line;
	if (A.size() != 0)
	{
		A.clear();
	}
	if (b.size() != 0)
	{
		b.clear();
	}
	if (file.is_open())
	{
		int line_count = 0;
		while (std::getline(file, line))
		{
			std::sregex_iterator it(line.begin(), line.end(), reg);
			std::sregex_iterator end;
			for (; it != end; ++it)
			{
				match = *it;

			}
		}
	}
}

static void inverse_matrix(std::vector<std::vector<double>>& matrix)
{

}


export void zad6()
{

}