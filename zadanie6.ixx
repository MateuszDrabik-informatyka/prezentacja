module;
#include <vector>
#include <thread>
#include <ranges>
#include <fstream>
#include <algorithm>
#include <regex>
#include <string>
#include <iostream>
#include <iomanip>
export module zadanie6;

static void determinant(std::vector<std::vector<double>> a, double& det)
{
	int n = a.size();
	det = 1;
	for (int i = 0; i < n; ++i) {
		int k = i;
		for (int j = i + 1; j < n; ++j)
		{
			if (abs(a[j][i]) > abs(a[k][i]))
			{
				k = j;
			}
		}
		if (abs(a[k][i]) < 0)
		{
			det = 0;
			break;
		}
		swap(a[i], a[k]);
		if (i != k)
		{
			det = -det;
		}
		det *= a[i][i];
		for (int j = i + 1; j < n; ++j)
		{
			a[i][j] /= a[i][i];
		}
		for (int j = 0; j < n; ++j)
		{
			if (j != i && abs(a[j][i]) > 0)
			{
				for (int k = i + 1; k < n; ++k)
				{
					a[j][k] -= a[i][k] * a[j][i];
				}
			}
		}
	}
}
static void transpose_matrix(std::vector<std::vector<double>>& matrix)
{
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			std::swap(matrix[i][j], matrix[j][i]);
		}
	}
}
std::vector<std::vector<double>> slice(const std::vector<std::vector<double>>& in, int x, int y)
{
	std::vector<std::vector<double>> temp(in.size()-1 ,std::vector<double> (in.size()-1));
	int i_ = 0;
	for (int i = 0; i < in.size();++i)
	{
		if (i == x)
		{
			continue;
		}
		int j_ = 0;
		for (int j = 0; j < in.size(); ++j)
		{
			if (j == y)
			{
				continue;
			}
			temp[i_][j_] = in[i][j];
			++j_;
		}
		++i_;
	}
	return temp;
}

static void adjoint_matrix(const std::vector<std::vector<double>>& in, std::vector<std::vector<double>>& out)
{
	std::vector<std::jthread> th;
	for (int i = 0; i < in.size(); ++i)
	{
		for (int j = 0; j < in.size(); ++j)
		{
			th.push_back(std::jthread([i,j,&in,&out]()
				{
					std::vector<std::vector<double>> t = slice(in, i, j);
					determinant(t, out[i][j]);
					out[i][j] *= pow(-1, j + i + 2);
					std::this_thread::yield();
				}));
		}
	}
}

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

static void inverse_matrix(const std::vector<std::vector<double>>& in, std::vector<std::vector<double>>& out)
{
	double det;
	std::vector<std::vector<double>> adj_matrix(in.size(),std::vector<double> (in.size()));
	std::thread det_t(determinant, in, std::ref(det));
	std::thread adj_t(adjoint_matrix, in, std::ref(adj_matrix));
	det_t.join();
	adj_t.join();
	transpose_matrix(adj_matrix);
	
	for (auto& i : adj_matrix)
	{
		for (auto& j : i)
		{
			j /= det;
		}
	}
	out = adj_matrix;
}

std::vector<std::vector<double>> multiple_matrix(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b)
{
	if (a[0].size() == b.size()&& b.size()>0)
	{
		std::vector<std::vector<double>> c(a.size(), std::vector<double>(b[0].size(),0.0));
		for (int i = 0; i < a.size(); ++i)
		{
			for (int j = 0; j < b[0].size(); ++j)
			{
				for (int k = 0; k < a[0].size(); ++k)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		return c;
	}
}

export void zad6()
{
	//Ax=b
	//x=A^(-1) b
	std::ifstream file("matrix.txt");
	std::vector < std::vector<double>> A;
	std::vector < std::vector<double>> A1(3,std::vector<double>(3));
	std::vector < std::vector<double>> b;
	std::vector < std::vector<double>> x;
	get_matrix(A, b, file);
	inverse_matrix(A, A1);
	x = multiple_matrix(A1, b);
	for (auto& i : x)
	{
		for (auto& j : i)
		{
			std::cout<<std::setprecision(4)<<std::setw(8) << j << " ";
		}
		std::cout << std::endl;
	}
}