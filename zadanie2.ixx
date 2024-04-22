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

#define THREAD_COUNT 4

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
				std::lock_guard<std::mutex> lock(mtx);
				map[match[1]].push_back(val);
			}
		}
	}
}

static void write_to_file(std::ofstream& file, const std::unordered_map<std::string, std::vector<double>>& map,
	std::unordered_map<std::string, std::vector<double>>::const_iterator begin, std::unordered_map<std::string, std::vector<double>>::const_iterator end,std::mutex& mtx)
{
	if (file.is_open())
	{
		for (;begin!=end;++begin)
		{
			std::lock_guard<std::mutex> lock(mtx);
			double sum = 0;
			sum = std::accumulate(begin->second.begin(), begin->second.end(),0.0);
			file << std::left << std::setw(30) << begin->first << std::setw(30) << sum / begin->second.size()
				<< std::setw(30) << std::ranges::min(begin->second) << std::setw(30) << std::ranges::max(begin->second) << std::endl;
		}
	}
}
export void zad2()
{
	std::unordered_map<std::string, std::vector<double>> stations;
	std::ifstream input{ std::filesystem::path{"weather_stations.csv"} };
	std::ofstream output{ std::filesystem::path("weather_stations_results.txt") };
	std::mutex mtx;
	std::vector<std::thread> read;
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		read.push_back(std::thread(read_from_file, std::ref(input), std::ref(stations), std::ref(mtx)));
	}
	for (auto& i : read)
	{
		i.join();
	}
	std::vector<std::jthread> write;
	std::unordered_map<std::string, std::vector<double>>::const_iterator begin = stations.begin();
	int stations_count = stations.size();
	output << std::left << std::setw(30) << "Miejscowosc" << std::setw(30) << "Srednia" << std::setw(30) << "Min" << std::setw(30) << "Max" << std::endl;
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		write.push_back(std::jthread(write_to_file, std::ref(output), std::ref(stations),std::next(begin,i*stations_count/ THREAD_COUNT),
			std::next(begin, (i+1) * stations_count / THREAD_COUNT),std::ref(mtx)));
	}
}