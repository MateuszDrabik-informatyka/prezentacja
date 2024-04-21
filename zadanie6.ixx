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
			std::vector<double> temp;
			for (; it != end; ++it)
			{
				match = *it;
				temp.push_back(std::stod(match[0]));
			}
			std::vector<double> temp_b;
			temp_b.push_back(temp[temp.size() - 1]);
			temp.pop_back();
			A.push_back(temp);
			b.push_back(temp_b);
		}
	}
}

static void inverse_matrix(std::vector<std::vector<double>>& matrix)
{

}

static void det(const std::vector<std::vector<double>>& matrix, double& det)
{
	if (matrix.size() == matrix[0].size())
	{
		if (matrix.size() == 1)
		{
			det = matrix[0][0];
			return;
		}
		if (matrix.size() == 2)
		{
			det = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
			return;
		}
	}
}

export void zad6()
{
	std::ifstream file("matrix.txt");
	std::vector < std::vector<double>> A;
	std::vector < std::vector<double>> b;
	get_matrix(A, b, file);
	for (auto& i : A)
	{
		for (auto& j : i)
		{
			std::cout << j << " ";
		}
		std::cout << std::endl;
	}
	for (auto& i : b)
	{
		for (auto& j : i)
		{
			std::cout << j << " ";
		}
		std::cout << std::endl;
	}
}