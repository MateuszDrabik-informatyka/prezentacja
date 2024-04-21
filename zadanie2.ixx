module;
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <regex>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <utility>
#include <queue>
#include <ranges>
export module zadanie2;

static void read_from_file(std::ifstream& file, std::unordered_map<std::string, std::vector<double>>& map, std::mutex& mtx)
{
	std::regex reg{ "([[a-zA-Z\\s']+);([0-9.-]*)" };
	std::smatch match;
	std::string line;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (std::regex_match(line, match, reg))
			{
				double val = std::stod(match[2]);
				mtx.lock();
				map[match[1]].push_back(val);
				mtx.unlock();
			}
		}
	}
}

static void write_to_file(std::ofstream& file, std::unordered_map<std::string, std::vector<double>>& map)
{
	if (file.is_open())
	{
		file << std::left << std::setw(30) << "Miejscowosc" << std::setw(30) << "Srednia" << std::setw(30) << "Min" << std::setw(30)<<"Max" << std::endl;
		for (auto& i: map)
		{
			double sum = 0;
			sum = std::accumulate(i.second.begin(), i.second.end(),0.0);
			file << std::left << std::setw(30) << i.first << std::setw(30) << sum / i.second.size()
				<< std::setw(30) << std::ranges::min(i.second) << std::setw(30) << std::ranges::max(i.second) << std::endl;
		}
	}
}
export void zad2()
{
	std::unordered_map<std::string, std::vector<double>> stations;
	std::ifstream input{ std::filesystem::path{"weather_stations.csv"} };
	std::ofstream output{ std::filesystem::path("weather_stations_results.txt") };
	std::mutex mtx;
	std::vector<std::jthread> read;
	for (size_t i = 0; i < 4; i++)
	{
		read.push_back(std::jthread(read_from_file, std::ref(input), std::ref(stations), std::ref(mtx)));
	}
	for (auto& i : read)
	{
		i.join();
	}
	std::jthread write (write_to_file, std::ref(output), std::ref(stations));
}